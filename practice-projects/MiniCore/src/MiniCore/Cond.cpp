#include "MiniCore/Cond.h"

namespace MiniCore
{
    Cond::Cond()
    {
        pthread_cond_init(&_cond, nullptr);
    }

    Cond::~Cond()
    {
        pthread_cond_destroy(&_cond);
    }

    void Cond::Wait(mutex& lock)
    {
        pthread_cond_wait(&_cond, lock.get());
    }

    void Cond::NotifyOne()
    {
        pthread_cond_signal(&_cond);
    }

    void Cond::NotifyAll()
    {
        pthread_cond_broadcast(&_cond);
    }
}