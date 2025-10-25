#include "icelog/TimeUtil.h"

namespace icelog
{
    std::string GetCurrentTime()
    {
        time_t currtime = time(nullptr);

        struct tm currtm;
        localtime_r(&currtime, &currtm);

        char timebuffer[64];
        snprintf(timebuffer, sizeof(timebuffer), "%4d-%02d-%02d %02d:%02d:%02d",
                 currtm.tm_year + 1900,
                 currtm.tm_mon + 1,
                 currtm.tm_mday,
                 currtm.tm_hour,
                 currtm.tm_min,
                 currtm.tm_sec);

        return timebuffer;
    }
}