#include "MiniCore/PrintTask.h"

namespace MiniCore
{
    PrintTask::PrintTask(int id) 
        : Task("PrintTask"), _id(id) {}

    void PrintTask::Execute()
    {
        int cnt = 3;
        while(cnt){
            std::cout << "执行打印任务: " << cnt << std::endl;
            cnt--;
        }
    }
}