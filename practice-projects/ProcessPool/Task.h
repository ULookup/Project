#ifndef __TASK_H__
#define __TASK_H__

#include <functional>
#include <vector>
#include <iostream>

using task_t = std::function<void()>; 

void Download();

void MySQL();

void Sync();

void Log();

class Init
{
public:
    Init();
};

extern Init GlobalInitTask;        // 全局初始化对象，负责任务注册

extern std::vector<task_t> tasks;  // 全局任务列表
#endif