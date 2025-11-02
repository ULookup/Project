#pragma once

#include "core/Task/HttpTask.h"
#include "core/ThreadPool/ThreadPool.hpp"
#include "net/InetAddress/InetAddress.h"
#include "net/Socket/Socket.h"
#include "lib/IceLog/Logger.h"
#include <iostream>
#include <functional>

using callback_t = std::function<std::string(std::string& buffer)>;

class Server
{
public:
    Server(uint16_t port);

    void Run();

    void HandlerRequest(std::shared_ptr<isframe::net::Socket> socket, isframe::net::InetAddress addr);

    ~Server() = default;
private:
    bool _isrunning;
    int _port;
    std::unique_ptr<isframe::net::Socket> _listensocket;
    isframe::core::ThreadPool<isframe::core::HttpTask> _threadpool;
};