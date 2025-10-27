#include "MiniCore/Thread.h"

namespace MiniCore
{
    const std::string& defaultThreadname = "UnnamedThread";

    Thread::Thread(func_t func, const std::string& name)
        : _name(name), _func(func), _is_running(false) 
        {
            EnableConsoleSink();
            LOG_INFO << "Create thread object successfully!";
        }
    
    void* Thread::start_routine(void* args)
    {
        Thread* self = static_cast<Thread*>(args);
        self->_is_running = true;
        self->_func(self->_name);
        pthread_exit((void*)0);
    }

    void Thread::Start()
    {
        int ret = pthread_create(&_tid, nullptr, start_routine, this);
        if(ret == 0){
            LOG_INFO << _name <<" Thread created successfully!";
        }
        else{
            LOG_FATAL << _name << "Thread Start failed! Error number: " << ret;
        }
    }

    void Thread::Stop()
    {
        int ret = pthread_cancel(_tid);
        if(ret != 0){
            LOG_FATAL << _name << " Thread Stop failed! Error number: " << ret; 
        }
    }

    void Thread::Join()
    {
        if(!_is_running){
            return;
        }
        int ret = pthread_join(_tid, nullptr);
        if(ret == 0){
            LOG_INFO << _name << " Thread join successfully!";
        }
        else{
            LOG_FATAL << _name << " Thread join failed! Error number: " << ret;
        }
    }
}