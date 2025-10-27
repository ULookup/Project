#include "MiniCore/Mutex.h"

namespace MiniCore
{
    mutex::mutex() { pthread_mutex_init(&_mutex,nullptr); }

    mutex::~mutex() { pthread_mutex_destroy(&_mutex); }

    int mutex::lock() { return pthread_mutex_lock(&_mutex); }

    int mutex::unlock() { return pthread_mutex_unlock(&_mutex); }

    pthread_mutex_t* mutex::get() { return &_mutex; }

    LockGuard::LockGuard(mutex& mutex) : _mutex(mutex) { _mutex.lock(); }

    LockGuard::~LockGuard() { _mutex.unlock(); }
}