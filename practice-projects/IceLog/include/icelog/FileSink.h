#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include "LogSink.h"
#include "Mutex.h"

namespace icelog
{
    class FileSink : public LogSink
    {
    public:
        FileSink(const std::string &path = "log", const std::string &filename = "log.txt");

        void SyncLog(const std::string &logmessage) override;

        ~FileSink();

    private:
        std::string _path;
        std::string _filename;
        mutex _lock;
    };
}