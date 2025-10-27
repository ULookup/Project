#ifndef __THREAD_H__
#define __THREAD_H__

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include "icelog/Logger.h"

namespace MiniCore
{
    typedef std::function<void(const std::string& name)> func_t;

    extern const std::string& defaultThreadname;

    class Thread
    {
    public:
        Thread(func_t func, const std::string& threadname = defaultThreadname);

        static void* start_routine(void* args);

        void Start();

        void Stop();

        void Join();

        ~Thread() = default;
    private:
        bool _is_running;
        pthread_t _tid;
        std::string _name;
        func_t _func;
    };
}

#endif