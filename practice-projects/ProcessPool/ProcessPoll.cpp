#include "ProcessPoll.h"

// Channel类成员函数实现

Channel::Channel(int fd, const std::string &name, pid_t id) : 
    _writefd(fd),
    _name(name),
    _sub_target(id)
{
}

Channel::~Channel() {}

int Channel::GetFd() { return _writefd; }

std::string Channel::GetName() { return _name; }

pid_t Channel::GetTarget() { return _sub_target; }

void Channel::Close() { close(_writefd); }

void Channel::Wait()
{
    pid_t retpid = waitpid(_sub_target, nullptr, 0);
    if (retpid == -1)
    {
        perror("channel waitpid error\n");
        exit(EXIT_FAILURE);
    }
}

// ProcessPool类成员函数实现

void ProcessPool::CtrlSubProcessHelper(int &index)
{
    // 1.选择一个通道
    int who = index;
    index++;
    index %= _channels.size();

    // 2.选择一个任务（任务码）

    int task_code = rand() % tasks.size();

    // 3.将任务码通过通道发送给子进程
    std::cout << "选择信道: " << _channels[who].GetName() << ", 目标子进程: " << _channels[who].GetTarget() << std::endl;
    write(_channels[who].GetFd(), &task_code, sizeof(task_code)); // 将任务码发送给子进程

#ifdef DEBUG
    sleep(1); // 调试模式下延时1秒，方便观察输出结果
#endif
}

ProcessPool::ProcessPool(int process_number = global_default_process_number)
    : _process_number(process_number)
{
}

bool ProcessPool::InitProcessPool(callback_func callback)
{


    for (int i = 0; i < _process_number; i++)
    {
        #ifdef DEBUG
        sleep(1);
        #endif
        // 1.创建管道
        int pipefd[2] = {0};
        int n = pipe(pipefd);
        if (n < 0)
        {
            perror("pipe error\n");
            return false;
        }
        //  2.创建子进程
        pid_t id = fork();
        if (id < 0)
        {
            perror("fork error\n");
            return false;
        }
        else if (id == 0)
        {
            // 子进程运行
            for (auto &e : _channels)
            {
                e.Close(); // 关闭子进程从父进程继承下来的无关历史信道
            }
            // 3.关闭子进程不需要的写端
            close(pipefd[1]); // 关闭子进程写端

            // 4.执行回调函数
            callback(pipefd[0]); // 执行回调函数
            exit(EXIT_SUCCESS);  // 子进程执行完毕后正常结束
        }
        // 父进程运行
        close(pipefd[0]); // 关闭父进程读端
        std::string name = "channel-" + std::to_string(i);
        _channels.emplace_back(pipefd[1], name, id); // 将创建的信道加入信道列表
    }
    return true;
}

void ProcessPool::PollingCtrlSubProcess()
{
    int index = 0;
    while (true)
    {
        CtrlSubProcessHelper(index); // 轮询控制子进程
    }
}

void ProcessPool::PollingCtrlSubProcess(int count)
{
    int index = 0;
    while (count)
    {
        CtrlSubProcessHelper(index); // 轮询控制子进程
        count--;
    }
}

void ProcessPool::WaitSubProcess()
{
    for (auto &e : _channels)
    {
        e.Close(); // 关闭信道
        e.Wait();  // 等待子进程结束
    }
}