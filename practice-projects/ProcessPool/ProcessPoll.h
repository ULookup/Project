#ifndef __PROCESSPOOL_H__
#define __PROCESSPOOL_H__

#define DEBUG

#include "Task.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const int global_default_process_number = 4;       // 默认子进程数量
using callback_func = std::function<void(int fd)>; // 统一回调函数类型

// 定义一个信道类
class Channel
{
public:
    Channel();                                          // 默认构造函数
    Channel(int fd, const std::string &name, pid_t id); // 传参构造函数
    ~Channel();                                         // 默认析构函数

    int GetFd();           // 获取文件描述符
    std::string GetName(); // 获取信道名称
    pid_t GetTarget();     // 获取目标子进程pid

    void Close(); // 关闭信道
    void Wait();  // 等待子进程结束
private:
    int _writefd;      // 写端文件描述符
    std::string _name; // 信道名称
    pid_t _sub_target; // 目标子进程pid
};

// 定义一个进程池类
class ProcessPool
{
private:
    void CtrlSubProcessHelper(int &index); // 控制子进程核心函数
public:
    ProcessPool(int process_number); // 默认构造函数

    bool InitProcessPool(callback_func callback); // 初始化进程池

    void PollingCtrlSubProcess(); // 轮询控制子进程 - 无限循环

    void PollingCtrlSubProcess(int count); // 轮询控制子进程 - 指定次数

    void WaitSubProcess(); //
private:
    std::vector<Channel> _channels; // 所有信道
    int _process_number;            // 子进程数量
};

#endif