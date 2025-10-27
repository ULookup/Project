#ifndef __COND_H__
#define __COND_H__

#include "MiniCore/Mutex.h"
#include <pthread.h>

namespace MiniCore
{
    class Cond
    {
    public:
        Cond();
        ~Cond();

        void Wait(mutex& lock);
        void NotifyOne();
        void NotifyAll();
    private:
        pthread_cond_t _cond;
    };
}

#endif