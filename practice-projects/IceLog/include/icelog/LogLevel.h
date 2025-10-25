#pragma once

#include "string"

namespace icelog
{
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string LevelToString(LogLevel level);
}