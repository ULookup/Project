#ifndef __SAFEQUEUE_H__
#define __SAFEQUEUE_H__

#include "MiniCore/Mutex.h"
#include "MiniCore/Cond.h"
#include <vector>

namespace MiniCore
{
    const size_t g_defaultCapacity = 64;

    template<class T>
    class SafeQueue
    {
    public:
        SafeQueue(size_t capacity = g_defaultCapacity);
        ~SafeQueue() = default;

        void push(const T& item);
        bool pop(T& item);

        bool empty() const;
        bool full() const;
    private:
        std::vector<T> _buffer;
        size_t _capacity;

        size_t _producter;
        size_t _consumer;
        size_t _count;
    
        mutable mutex _lock;
        Cond _not_empty;
        Cond _not_full;
    };

}

#include "MiniCore/SafeQueue.tpp"

#endif