#include "core/Task/HttpTask.h"

namespace isframe::core
{
    static const std::string linesep = "\r\n";
    static const std::string innersep1 = " ";
    static const std::string innersep2 = ": ";
    static const std::string webroot = "./wwwroot";
    static const std::string defaulthome = "index.html";
    static const std::string html_404 = "404.html";
    static const std::string suffixsep = ".";
    static const std::string argssep = "?";

    static std::mutex msg_mutex;

    // 转义 JSON 字符串中的特殊字符
    static std::string EscapeJson(const std::string &input)
    {
        std::ostringstream ss;
        for (char c : input)
        {
            switch (c)
            {
            case '\"':
                ss << "\\\"";
                break;
            case '\\':
                ss << "\\\\";
                break;
            case '\b':
                ss << "\\b";
                break;
            case '\f':
                ss << "\\f";
                break;
            case '\n':
                ss << "\\n";
                break;
            case '\r':
                ss << "\\r";
                break;
            case '\t':
                ss << "\\t";
                break;
            default:
                if ((unsigned char)c < 0x20)
                {
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                }
                else
                    ss << c;
            }
        }
        return ss.str();
    }

    void HttpTask::InitApiHandlers()
    {
        const std::string file = "./wwwroot/data/messages.txt";
        std::filesystem::create_directories("./wwwroot/data");

        _handlers[webroot + "/api/message"] = [file](HttpRequest &req) -> HttpResponse
        {
            HttpResponse resp;

            // --- DELETE: 清空留言文件 ---
            if (req.GetMethod() == "DELETE")
            {
                std::lock_guard<std::mutex> lock(msg_mutex);
                std::ofstream ofs(file, std::ios::trunc);
                ofs.close();
                resp.SetCode(200);
                resp.SetHeader("Content-Type", "application/json; charset=utf-8");
                resp.SetBody("{\"msg\":\"cleared\"}");
                return resp;
            }

            // --- POST: 写入新留言 ---
            if (req.GetMethod() == "POST")
            {
                std::lock_guard<std::mutex> lock(msg_mutex);
                std::string body = req.GetBody();

                std::string user, text;
                auto u1 = body.find("\"user\":\"");
                if (u1 != std::string::npos)
                {
                    u1 += 8;
                    auto u2 = body.find("\"", u1);
                    user = body.substr(u1, u2 - u1);
                }
                auto t1 = body.find("\"text\":\"");
                if (t1 != std::string::npos)
                {
                    t1 += 8;
                    auto t2 = body.find("\"", t1);
                    text = body.substr(t1, t2 - t1);
                }
                if (user.empty())
                    user = "匿名用户";

                // 时间：可读的 + 秒级时间戳
                auto now_tp = std::chrono::system_clock::now();
                auto now = std::chrono::system_clock::to_time_t(now_tp);
                auto epoch = std::chrono::duration_cast<std::chrono::seconds>(now_tp.time_since_epoch()).count();

                std::stringstream ss_time;
                ss_time << std::put_time(std::localtime(&now), "%H:%M:%S");

                std::ofstream ofs(file, std::ios::app);
                if (ofs.is_open())
                {
                    // 原来是 3 段，这里变 4 段：user||text||HH:MM:SS||1730534660
                    ofs << user << "||" << text << "||" << ss_time.str() << "||" << epoch << "\n";
                    ofs.close();
                }

                resp.SetCode(200);
                resp.SetHeader("Content-Type", "application/json; charset=utf-8");
                resp.SetBody("{\"msg\":\"ok\"}");
                return resp;
            }

            // --- GET: 读取留言列表（支持 ?since=xxx 增量获取） ---
            std::lock_guard<std::mutex> lock(msg_mutex);
            std::ifstream ifs(file);

            // 1. 取出 GET 上的查询参数（你当前的 HttpRequest 在 GET 时把 query 放在 body 里了）
            long long since_ts = 0;
            std::string q = req.GetBody(); // 这里其实是 ? 后面的整串
            // 例子：since=1730534660
            if (!q.empty())
            {
                auto pos = q.find("since=");
                if (pos != std::string::npos)
                {
                    pos += 6;
                    std::string v = q.substr(pos);
                    since_ts = std::stoll(v);
                }
            }

            std::stringstream buf;
            std::string line;
            buf << "[";
            bool first = true;

            while (std::getline(ifs, line))
            {
                // 按你现在的格式拆：user||text||time||epoch
                size_t p1 = line.find("||");
                size_t p2 = line.find("||", p1 + 2);
                size_t p3 = line.find("||", p2 + 2); // 可能不存在（老数据）

                std::string user = "匿名用户";
                std::string content;
                std::string time;
                long long ts = 0;

                if (p1 != std::string::npos)
                    user = line.substr(0, p1);
                if (p1 != std::string::npos && p2 != std::string::npos)
                    content = line.substr(p1 + 2, p2 - p1 - 2);
                if (p2 != std::string::npos)
                {
                    if (p3 != std::string::npos)
                    {
                        // 新格式：...||HH:MM:SS||1730534660
                        time = line.substr(p2 + 2, p3 - p2 - 2);
                        std::string ts_str = line.substr(p3 + 2);
                        if (!ts_str.empty())
                            ts = std::stoll(ts_str);
                    }
                    else
                    {
                        // 老格式：...||HH:MM:SS
                        time = line.substr(p2 + 2);
                    }
                }

                // 如果前端带了 since，就只返回“更新后的”
                if (since_ts > 0 && ts > 0 && ts <= since_ts)
                {
                    continue;
                }

                if (!first)
                    buf << ",";
                first = false;
                buf << "{\"user\":\"" << EscapeJson(user)
                    << "\",\"text\":\"" << EscapeJson(content)
                    << "\",\"time\":\"" << time << "\""
                    << ",\"ts\":" << ts
                    << "}";
            }

            buf << "]";
            ifs.close();

            resp.SetCode(200);
            resp.SetHeader("Content-Type", "application/json; charset=utf-8");
            resp.SetBody(buf.str());
            return resp;
        };

        // ===============================================
        // 在线用户模块
        // ===============================================

        static std::unordered_map<std::string, time_t> onlineUsers;
        static std::mutex onlineMutex;
        const int TIMEOUT = 30; // 30秒未心跳即视为离线

        // --- POST: 心跳上报 ---
        _handlers[webroot + "/api/heartbeat"] = [&](HttpRequest &req) -> HttpResponse
        {
            HttpResponse resp;
            std::string body = req.GetBody();
            std::string user;

            auto u1 = body.find("\"user\":\"");
            if (u1 != std::string::npos)
            {
                u1 += 8;
                auto u2 = body.find("\"", u1);
                user = body.substr(u1, u2 - u1);
            }
            if (user.empty())
                user = "匿名用户";

            {
                std::lock_guard<std::mutex> lock(onlineMutex);
                onlineUsers[user] = time(nullptr);
            }

            resp.SetCode(200);
            resp.SetHeader("Content-Type", "application/json; charset=utf-8");
            resp.SetBody("{\"msg\":\"heartbeat ok\"}");
            return resp;
        };

        // --- GET: 获取在线用户 ---
        _handlers[webroot + "/api/online"] = [&](HttpRequest &req) -> HttpResponse
        {
            HttpResponse resp;
            std::ostringstream ss;
            ss << "[";
            bool first = true;
            time_t now = time(nullptr);

            {
                std::lock_guard<std::mutex> lock(onlineMutex);
                for (auto it = onlineUsers.begin(); it != onlineUsers.end();)
                {
                    if (now - it->second > TIMEOUT)
                        it = onlineUsers.erase(it);
                    else
                    {
                        if (!first)
                            ss << ",";
                        first = false;
                        ss << "\"" << EscapeJson(it->first) << "\"";
                        ++it;
                    }
                }
            }

            ss << "]";
            resp.SetCode(200);
            resp.SetHeader("Content-Type", "application/json; charset=utf-8");
            resp.SetBody(ss.str());
            return resp;
        };
    }

    std::string isframe::core::HttpRequest::ReadOneLine(std::string &reqstr, bool *status)
    {
        auto pos = reqstr.find(linesep);
        if (pos == std::string::npos)
        {
            *status = false;
            return std::string();
        }
        *status = true;
        auto line = reqstr.substr(0, pos);
        reqstr.erase(0, pos + linesep.size());
        return line;
    }

    void isframe::core::HttpRequest::ParseReqLine(std::string &reqline)
    {
        std::stringstream ss(reqline);
        ss >> _method >> _uri >> _httpversion;
    }

    void isframe::core::HttpRequest::BuildKV(std::string &reqline, std::string *K, std::string *V)
    {
        auto pos = reqline.find(innersep2);
        if (pos == std::string::npos)
        {
            *K = *V = std::string();
            return;
        }
        *K = reqline.substr(0, pos);
        *V = reqline.substr(pos + innersep2.size());
    }

    bool isframe::core::HttpRequest::Deserialize(std::string &reqstr)
    {
        bool status = true;
        std::string reqline = ReadOneLine(reqstr, &status);
        if (!status)
        {
            return false;
        }

        std::cout << "##############" << reqline << "##############" << std::endl;

        ParseReqLine(reqline);

        while (true)
        {
            status = true;
            reqline = ReadOneLine(reqstr, &status);
            if (status && !reqline.empty())
            {
                std::string k, v;
                BuildKV(reqline, &k, &v);
                if (k.empty() || v.empty())
                {
                    continue;
                }
                _req_headers.insert(std::make_pair(k, v));
            }
            else if (status)
            {
                _blank_line = linesep;
                break;
            }
            else
            {
                std::cout << "非法请求" << std::endl;
                break;
            }
        }

        _path = webroot;
        _path += _uri;

        if (_uri == "/")
        {
            _path += defaulthome;
        }
        // std::cout << "_path: " << _path << std::endl;
        if (_method == "GET")
        {
            auto pos = _path.find(argssep);
            if (pos != std::string::npos)
            {
                _req_body = _path.substr(pos + argssep.size());
                _path = _path.substr(0, pos);
            }
        }
        else if (_method == "POST")
        {
            _req_body = reqstr;
        }

        return true;
    }

    std::string isframe::core::HttpRequest::GetPath()
    {
        return _path;
    }

    void isframe::core::HttpRequest::SetPath(const std::string &path)
    {
        _path = path;
    }

    std::string isframe::core::HttpRequest::Suffix()
    {
        if (_path.empty())
        {
            return std::string();
        }
        else
        {
            auto pos = _path.rfind(suffixsep);
            if (pos == std::string::npos)
            {
                return std::string();
            }
            else
            {
                return _path.substr(pos);
            }
        }
    }

    //////////////////////////////////////////////////////////

    std::string isframe::core::HttpResponse::CodeToDesc(int code)
    {
        switch (code)
        {
        case 200:
            return "OK";
        case 400:
            return "Bad Request";
        case 404:
            return "Not Found";
        case 301:
            return "Moved Permanently";
        case 302:
            return "See Other";
        case 307:
            return "Temporary Redirect";
        default:
            return "UnKown";
        }
    }

    isframe::core::HttpResponse::HttpResponse() : _httpversion("HTTP/1.1"), _blank_line(linesep) {}

    std::string isframe::core::HttpResponse::Serialize()
    {
        std::string respstr = _httpversion + innersep1 + std::to_string(_code) + innersep1 + _desc + linesep;

        if (_resp_headers.find("Connection") == _resp_headers.end())
            _resp_headers["Connection"] = "close";

        _resp_headers["Content-Length"] = std::to_string(_resp_body.size());

        for (auto &elem : _resp_headers)
        {
            std::string line = elem.first + innersep2 + elem.second + linesep;
            respstr += line;
        }

        respstr += _blank_line;
        respstr += _resp_body;

        return respstr;
    }

    void isframe::core::HttpResponse::DeSerialize() { /* 响应不需要反序列化 */ }

    bool isframe::core::HttpResponse::ReadContent(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            std::cout << path << " 资源不存在!";
            return false;
        }

        // 定位到文件末尾获取文件大小
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg(); // 获取文件大小
        file.seekg(0, std::ios::beg);

        // 创建足够大的字符串来保存数据
        _resp_body.resize(static_cast<size_t>(fileSize));

        // 读取文件内容到字符串
        file.read(&_resp_body[0], fileSize);
        file.close();

        return true;
    }

    void isframe::core::HttpResponse::SetCode(int code)
    {
        if (code >= 100 && code < 600)
        {
            _code = code;
            _desc = CodeToDesc(_code);
        }
        else
        {
            std::cout << "非法的状态码: " << _code << std::endl;
        }
    }

    bool isframe::core::HttpResponse::SetHeader(const std::string &key, const std::string &value)
    {
        _resp_headers[key] = value;
        return true;
    }

    ///////////////////////////////////////////////////////

    std::string isframe::core::HttpTask::SuffixToDesc(const std::string &suffix)
    {
        if (suffix == ".html")
            return "text/html";
        else if (suffix == ".css")
            return "text/css";
        else if (suffix == ".js")
            return "application/x-javascript";
        else if (suffix == ".png")
            return "image/png";
        else if (suffix == ".jpg")
            return "image/jpeg";
        else if (suffix == ".txt")
            return "text/plain";
        else
            return "text/html";
    }

    void isframe::core::HttpTask::Register(const std::string &action, func_t handler)
    {
        std::string key = webroot;
        key += action;
        _handlers[key] = handler;
    }

    isframe::core::HttpTask::HttpTask(const std::string &reqstr, std::shared_ptr<isframe::net::Socket> socket)
        : _request(reqstr), _socket(socket)
    {
        LOG_DEBUG << "HttpTask构造 _socket.use_count()=" << _socket.use_count();
        InitApiHandlers();
    }

    std::string isframe::core::HttpTask::HandlerRequest(std::string &requeststr)
    {
        std::string respstr;
        HttpRequest req;
        std::cout << requeststr << std::endl;
        if (req.Deserialize(requeststr))
        {
            HttpResponse resp;
            // 1.交互式处理
            std::string target = req.GetPath();
            if (_handlers.find(target) != _handlers.end())
            {
                resp = _handlers[target](req);
            }
            else
            { // 2. 静态处理
                if (resp.ReadContent(req.GetPath()))
                {
                    std::string suffix = req.Suffix();
                    std::string mime_type_value = SuffixToDesc(suffix);
                    resp.SetHeader("Content-Type", mime_type_value);
                    resp.SetCode(200);
                }
                else
                {
                    resp.SetCode(302);
                    resp.SetHeader("Location", "/404.html");
                }
            }
            respstr = resp.Serialize();
            // std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
            // std::cout << respstr << std::endl;
            // std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
        }
        else
        {
            std::string body = "<h1>400 Bad Request</h1>";
            std::ostringstream oss;
            oss << "HTTP/1.1 400 Bad Request\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << body.size() << "\r\n"
                << "Connection: close\r\n\r\n"
                << body;
            return oss.str();
        }
        return respstr;
    }

    void isframe::core::HttpTask::Execute()
    {
        LOG_DEBUG << "准备进入任务执行函数";
        LOG_DEBUG << "Execute开始 _socket.use_count()=" << _socket.use_count();
        std::string send_str = HandlerRequest(_request);
        LOG_DEBUG << "请求处理完毕,准备发送响应";
        ssize_t n = _socket->SendTo(send_str);
        if (n == -1)
        {
            LOG_WARNING << "send failed, errno=" << errno << " msg=" << strerror(errno);
        }

        LOG_DEBUG << "send bytes = " << n << ", total = " << send_str.size();
        LOG_DEBUG << "发送响应完毕";
        _socket->Close();
        LOG_DEBUG << "关闭socket";
    }
}