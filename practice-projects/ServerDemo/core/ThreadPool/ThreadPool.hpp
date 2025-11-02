#pragma once

#include "lib/IceLog/Logger.h"
#include "core/SafeQueue/SafeQueue.hpp"
#include <thread>
#include <condition_variable>

namespace isframe::core
{
    const int g_defaultThreadNum = 8;

    template<class T>
    class ThreadPool
    {
    private:
        void Routine(const std::string& name);

        bool QueueIsEmpty();

    public:
        ThreadPool(int threadnum = g_defaultThreadNum);
        
        void Start();
        void Stop();
        void Wait();

        void Enqueue(std::shared_ptr<T> task);

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
    private:
        SafeQueue<std::shared_ptr<T>> _queue;
        
        std::vector<std::thread> _threads;
        int _thread_num;
        int _wait_thread_num;

        std::atomic<bool> _is_running;


    };

    template<class T>
    void ThreadPool<T>::Routine(const std::string& name)
    {
        while (_is_running) {
            std::shared_ptr<T> task;
            _queue.pop(task);  // SafeQueue 自带同步
            if (task){
                LOG_DEBUG << "线程: " << name << "即将执行任务";
                (*task)();
            }
        }
    }

     template<class T>
    bool ThreadPool<T>::QueueIsEmpty()
    {
        return _queue.empty();
    }

    template<class T>
    ThreadPool<T>::ThreadPool(int threadnum) :
        _thread_num(threadnum),
        _wait_thread_num(0),
        _is_running(false)
        {}

    template<class T>
    void ThreadPool<T>::Start()
    {
        _is_running = true;
        for (int i = 0; i < _thread_num; ++i){
            std::string name = "thread-" + std::to_string(i);
            _threads.emplace_back([this, name]() { this->Routine(name); });
            LOG_DEBUG << "创建线程成功: " << name;
        }
    }

    template<class T>
    void ThreadPool<T>::Stop()
    {
        if(!_is_running)
            return;
        _is_running = false;
        // 投递 thread_num 个空任务作为“毒丸”唤醒阻塞线程并让其退出
        for (int i = 0; i < _thread_num; ++i){
            _queue.push(nullptr);
        }
        for (auto& t : _threads){
            if (t.joinable()){
                t.join();
            }
        }
    }

    template<class T>
    void ThreadPool<T>::Wait()
    {
        for(auto& thread : _threads){
            thread.join();
        }
    }

    template<class T>
    void ThreadPool<T>::Enqueue(std::shared_ptr<T> task)
    {
        if(!_is_running){
            return;
        }
        _queue.push(task);
        LOG_DEBUG << "将http任务插入任务队列成功";
    }
}