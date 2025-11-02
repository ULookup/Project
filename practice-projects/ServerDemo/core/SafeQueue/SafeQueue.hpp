#pragma once

#include "lib/IceLog/Logger.h"
#include <vector>
#include <mutex>
#include <semaphore>

namespace isframe::core{

    const size_t g_defaultCapacity = 64;

    template<class T>
    class SafeQueue
    {
    public:
        SafeQueue(size_t capacity = g_defaultCapacity);
        ~SafeQueue() = default;

        void push(const T& item);
        void pop(T& item);

    private:
        size_t _capacity;
        std::vector<T> _buffer;

        int _producter;
        int _consumer;
    
        std::counting_semaphore<g_defaultCapacity> _spacesem;
        std::counting_semaphore<g_defaultCapacity> _tasksem;

        std::mutex _consumelock;
        std::mutex _productlock;
    };

    template<class T>
    isframe::core::SafeQueue<T>::SafeQueue(size_t capacity)
        : _capacity(capacity), _buffer(_capacity),
          _producter(0), _consumer(0),
          _spacesem(_capacity), _tasksem(0)
        {}

    template<class T>
    void isframe::core::SafeQueue<T>::push(const T& task)
    {   
        _spacesem.acquire(); 
        {
            std::lock_guard<std::mutex> lockguard(_productlock);
            _buffer[_producter++] = task;
            LOG_DEBUG << "任务已经进入队列";
            _producter %= _capacity;
        }
         _tasksem.release();
    }

    template<class T>
    void isframe::core::SafeQueue<T>::pop(T& task)
    {
        _tasksem.acquire();
        {
            std::lock_guard<std::mutex> lockguard(_consumelock);
            task = _buffer[_consumer++];
            LOG_DEBUG << "http任务被线程取出";
            _consumer %= _capacity;
        }
        _spacesem.release();
    }
}