#pragma once

#include <string>

namespace icelog
{
    class LogSink
    {
    public:
        virtual ~LogSink() = default;
        virtual void SyncLog(const std::string &logmessage) = 0;
    };
}