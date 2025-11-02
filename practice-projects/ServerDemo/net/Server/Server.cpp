#include "net/Server/Server.h"

Server::Server(uint16_t port)
    : _isrunning(false), _port(port)
    {
        _listensocket = std::make_unique<isframe::net::TcpSocket>(_port);
        _listensocket->Create();
        _listensocket->Bind(_port);
        _listensocket->Listen();
        LOG_DEBUG << "服务器被创建并开始监听";
    }

void Server::Run()
{
    LOG_DEBUG << "服务器正在运行";
   _isrunning = true;
   _threadpool.Start();
   while(_isrunning){
        isframe::net::InetAddress clientaddr;
        auto socket = _listensocket->Accept(clientaddr);
        if(socket == nullptr){
            continue;
        }
        //for log...
        LOG_DEBUG << "监听到并开始接收客户端请求";
        HandlerRequest(socket, clientaddr);
   }
   _isrunning = false;
}

void Server::HandlerRequest(std::shared_ptr<isframe::net::Socket> socket, isframe::net::InetAddress addr)
{
    LOG_DEBUG << "创建客户端请求缓冲区";
    std::string buffer;
    ssize_t n = 0;
    while (true) {
        std::string chunk;
        n = socket->Recv(chunk);
        if (n <= 0) {
            break;
        }
        buffer += chunk;
        if(buffer.find("\r\n\r\n") != std::string::npos){
            break;
        }
    }
    LOG_DEBUG << "收到客户端请求";
    std::cout << "//////////////// buffer ////////////////" << std::endl;
    std::cout << buffer << std::endl;
    std::cout << "////////////////   end  ////////////////" << std::endl;
   if(!buffer.empty()){
        LOG_DEBUG << "创建http任务成功";
        LOG_DEBUG << "Server侧 socket.use_count() = " << socket.use_count();
        _threadpool.Enqueue(std::make_shared<isframe::core::HttpTask>(buffer, socket));
        LOG_DEBUG << "Enqueue后 (仍在Server) socket.use_count() = " << socket.use_count();
    }else{
        socket->Close();
    }
}