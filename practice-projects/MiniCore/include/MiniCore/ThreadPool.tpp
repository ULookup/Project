#pragma once
#include "MiniCore/ThreadPool.h"
#include "icelog/Logger.h"

namespace MiniCore
{

    template <class T>
    ThreadPool<T>* ThreadPool<T>::_instance = nullptr;

    template <class T>
    mutex ThreadPool<T>::_singleton_lock;

    template<class T>
    void ThreadPool<T>::Routine(const std::string& name)
    {
        while(true){
            T task;
            {
                LockGuard Lockguard(_lock);
                while(QueueIsEmpty() && _is_running){
                    _wait_thread_num++;
                    _cond.Wait(_lock);
                    _wait_thread_num--;
                }
                if( !_is_running && QueueIsEmpty()){
                    LOG_INFO << "ThreadPool is empty or ThreadPool is not running. " << name << " exit.";
                    break;
                }
                _queue.pop(task);
            }
            task(); //执行任务，线程核心执行部分
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
        _is_running(false),
        _wait_thread_num(0)
        {
            for(int i = 0; i < _thread_num; i++){
                std::string threadname = "thread-" + std::to_string(i + 1);
                _threads.emplace_back( [this](const std::string& name){ this->Routine(name); }, threadname);
            }
            LOG_INFO << "Threads created successfully!";
        }

    template<class T>
    void ThreadPool<T>::Start()
    {
        if(_is_running)
            return;
        _is_running = true;
        for(auto& thread : _threads){
            thread.Start();
        }
        LOG_INFO << "Threads started successfully!";
    }

    template<class T>
    void ThreadPool<T>::Stop()
    {
        if(!_is_running)
            return;
        _is_running = false;
        if(_wait_thread_num){
            _cond.NotifyAll(); //唤醒等待的线程，如果线程池空了（没任务了）或者线程池已经关闭了，这些醒来的线程会自动执行退出动作
        }
        LOG_INFO << "Threads stopped successfully!";
    }

    template<class T>
    void ThreadPool<T>::Wait()
    {
        for(auto& thread : _threads){
            thread.Join();
        }
        LOG_INFO << "Threadpool wait successfully!";
    }

    template<class T>
    void ThreadPool<T>::Enqueue(const T& task)
    {
        if(!_is_running){
            LOG_ERROR << "ThreadPool is not running!";
            return;
        }
        _queue.push(task);

        return;
    }

    template<class T>
    ThreadPool<T>* ThreadPool<T>::GetInstance()
    {
        {
            if(!_instance){
                LockGuard lockguard(_singleton_lock);
                if(!_instance){
                    _instance = new ThreadPool<T>();
                    LOG_INFO << "Threadpool is used for the first time. Create and init threadpool!";

                    _instance->Start();
                }
            }
        }
        return _instance;
    }
}
