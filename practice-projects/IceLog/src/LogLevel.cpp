#include "icelog/LogLevel.h"

namespace icelog
{
    std::string LevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return "Debug";
        case LogLevel::INFO:
            return "Info";
        case LogLevel::WARNING:
            return "Warning";
        case LogLevel::ERROR:
            return "Error";
        case LogLevel::FATAL:
            return "Fatal";
        default:
            return "Undefined";
        }
    }
}