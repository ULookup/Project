#ifndef __PRINTTASK_H__
#define __PRINTTASK_H__

#include "MiniCore/Task.h"
#include <iostream>

namespace MiniCore
{
    class PrintTask : public Task
    {
    public:
        PrintTask(int id = 1);

        void Execute();

    private:
        int _id;
    };
}

#endif