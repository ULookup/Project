#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "MiniCore/SafeQueue.h"
#include "MiniCore/Thread.h"
#include "MiniCore/PrintTask.h"
#include "MiniCore/Mutex.h"
#include "MiniCore/Cond.h"
#include "icelog/Logger.h"
#include <vector>
#include <string>

namespace MiniCore
{
    const int g_defaultThreadNum = 4;

    template<class T>
    class ThreadPool
    {
    private:
        void Routine(const std::string& name);

        bool QueueIsEmpty();
        ThreadPool(int threadnum = g_defaultThreadNum);
    public:
        void Start();
        void Stop();
        void Wait();

        void Enqueue(const T& task);

        static ThreadPool* GetInstance();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
    private:
        SafeQueue<T> _queue;
        
        std::vector<Thread> _threads;
        int _thread_num;
        int _wait_thread_num;

        bool _is_running;

        mutex _lock;
        Cond _cond;

        static ThreadPool<T>* _instance;
        static mutex _singleton_lock;
    };

}

#include "MiniCore/ThreadPool.tpp"

#endif