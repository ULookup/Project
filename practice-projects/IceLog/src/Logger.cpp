#include "icelog/Logger.h"

namespace icelog
{
    Logger logger;

    Logger::Logger() {}

    Logger::~Logger() {};

    void Logger::_EnableConsoleSink() { _sink = std::make_unique<ConsoleSink>(); };

    void Logger::_EnableFileSink() { _sink = std::make_unique<FileSink>(); }

    Logger::LogMessage::LogMessage(LogLevel level, std::string &filename, int line, Logger &logger)
        : _curr_time(GetCurrentTime()), _level(level), _pid(getpid()),
          _filename(filename), _line(line), _logger(logger)
    {
        std::stringstream ss;
        ss << "[" << _curr_time << "] "
           << "[" << LevelToString(_level) << "] "
           << "[" << _pid << "] "
           << "[" << _filename << "] "
           << "[" << _line << "]"
           << " - ";
        _loginfo = ss.str();
    }

    Logger::LogMessage::~LogMessage()
    {
        if (_logger._sink)
        {
            _logger._sink->SyncLog(_loginfo);
        }
    }

    Logger::LogMessage Logger::operator()(LogLevel level, std::string filename, int line) { return LogMessage(level, filename, line, *this); }
}