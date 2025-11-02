#include "core/Task/Task.h"
#include "net/Socket/Socket.h"
#include "lib/IceLog/Logger.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <iomanip>
#include <ctime>
#include <set>

namespace isframe::core
{
    class HttpRequest
    {
    private:
        std::string ReadOneLine(std::string &reqstr, bool *status);

        void ParseReqLine(std::string &reqline);

        void BuildKV(std::string &reqline, std::string *K, std::string *V);

    public:
        HttpRequest() noexcept = default;
        ~HttpRequest() noexcept = default;
        void Serialize();
        bool Deserialize(std::string &reqstr);

        std::string GetMethod() const { return _method; }
        std::string GetBody() const { return _req_body; }
        std::string GetUri() const { return _uri; }
        std::string GetPath();

        void SetPath(const std::string &path);

        std::string Suffix();

    private:
        std::string _method;
        std::string _uri;
        std::string _httpversion;
        std::unordered_map<std::string, std::string> _req_headers;
        std::string _blank_line;
        std::string _req_body;

        std::string _path;
    };

    class HttpResponse
    {
    private:
        std::string CodeToDesc(int code);

    public:
        HttpResponse();
        ~HttpResponse() = default;

        std::string Serialize();

        void DeSerialize();

        void SetBody(const std::string &body) { _resp_body = body; }
        void SetContentType(const std::string &type) { _resp_headers["Content-Type"] = type; }
        std::string GetBody() const { return _resp_body; }

        bool ReadContent(const std::string &path);

        void SetCode(int code);

        bool SetHeader(const std::string &key, const std::string &value);

    private:
        std::string _httpversion;
        int _code;
        std::string _desc;
        std::unordered_map<std::string, std::string> _resp_headers;
        std::string _blank_line;
        std::string _resp_body;
    };

    using func_t = std::function<HttpResponse(HttpRequest &)>;

    class HttpTask : public Task
    {
    private:
        std::unordered_map<std::string, func_t> _handlers;
    
        void Register(const std::string &action, func_t handler);

        std::string HandlerRequest(std::string &requeststr);

        std::string SuffixToDesc(const std::string &suffix);

        void InitApiHandlers();
    public:
        HttpTask(const std::string &reqstr, std::shared_ptr<isframe::net::Socket> socket);
        
        void Execute() override;  

    private:
        std::string _request;
        std::shared_ptr<isframe::net::Socket> _socket;
    };
}