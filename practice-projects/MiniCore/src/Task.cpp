#include "include/MiniCore/Task.h"

namespace MiniCore
{
    Task::Task(std::string name)
        : _taskname(name), _state(TaskState::CREATED) {}

    void Task::operator()() 
    {
        try{
            _state = TaskState::RUNNING;
            Execute();
            _state = TaskState::FINISHED;
        }
        catch(const std::exception& e){
            _state = TaskState::FAILED;
        }
    }

    void Task::OnStart() {}
    void Task::OnFinished() {}
    void Task::OnError() {}

    TaskState Task::GetState() const { return _state; }
}