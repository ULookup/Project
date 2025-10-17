#include "Task.h"

std::vector<task_t> tasks; // 全局任务列表

Init GlobalInitTask; // 全局初始化对象，负责任务注册

void Download() { std::cout << "执行了下载任务" << std::endl; }

void MySQL() { std::cout << "执行了MySQL任务" << std::endl; }

void Sync() { std::cout << "执行了数据同步任务" << std::endl; }

void Log() { std::cout << "执行了日志记录任务" << std::endl; }

Init::Init()
{
    tasks.push_back(Download);
    tasks.push_back(MySQL);
    tasks.push_back(Sync);
    tasks.push_back(Log);
}