#ifndef __TASK_H__
#define __TASK_H__

#include <functional>
#include <string>

namespace MiniCore
{
    enum class TaskState
    {
        CREATED,
        RUNNING,
        FINISHED,
        FAILED
    };

    class Task
    {
    public:
        Task(std::string name = "UnnamedTask");

        virtual ~Task() = default;

        void operator()();             //任务执行调用入口

        virtual void Execute() = 0;    //具体执行的任务函数

        //留给子类的钩子函数，可选择重写
        virtual void OnStart();
        virtual void OnFinished();
        virtual void OnError();

        TaskState GetState() const;
    private:
        std::string _taskname;
        TaskState _state;
    };
}

#endif