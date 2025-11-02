#pragma once

#include "LogSink.h"
#include "Mutex.h"

namespace icelog
{
    class ConsoleSink : public LogSink
    {
    public:
        ~ConsoleSink();
        void SyncLog(const std::string &logmessage) override;

    private:
        mutex _lock;
    };
}