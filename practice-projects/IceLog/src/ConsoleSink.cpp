#include "icelog/ConsoleSink.h"

namespace icelog
{
    ConsoleSink::~ConsoleSink() {}

    void ConsoleSink::SyncLog(const std::string &logmessage)
    {
        LockGuard lock(_lock);
        std::cout << logmessage << std::endl;
    }
}