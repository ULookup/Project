#include <iostream>
#include <unistd.h>
#include "MiniCore/ThreadPool.h"
#include "MiniCore/PrintTask.h"

using namespace MiniCore;

int main()
{
    // 1. 获取线程池单例实例
    auto* pool = ThreadPool<PrintTask>::GetInstance();

    // 2. 启动线程池
    pool->Start();
    std::cout << "[Main] ThreadPool started." << std::endl;

    // 3. 提交任务
    for (int i = 0; i < 10; ++i)
    {
        PrintTask task(i);  // 任务 ID
        pool->Enqueue(task);
        std::cout << "[Main] Enqueued task " << i << std::endl;
        usleep(100000); // 模拟任务间隔
    }

    // 4. 等待任务执行完成
    sleep(2);  // 等待线程执行任务
    pool->Stop();
    pool->Wait();

    std::cout << "[Main] ThreadPool stopped, all tasks finished." << std::endl;
    return 0;
}
