#include "ProcessPoll.h"
#include "Task.h"

using namespace std;

int main()
{
    ProcessPool pool(5);
    pool.InitProcessPool([](int fd)
                         {
        while(true){
            int code = -1;
            ssize_t n = read(fd, &code, sizeof(code));
            if(n == sizeof(code)){
                cout << "子进程被唤醒: " << getpid() << ", fd: " << fd << endl;
                if(code >=0 && code < tasks.size()){
                    tasks[code]();
                } 
                else {
                    cerr << "收到无效任务码" << endl;
                }
            }
            else if(n == 0){
                cout << "控制进程写端关闭,子进程退出: " << getpid() << endl;
                break;
            }
            else{
                cerr << "读取任务码失败" << endl;
                break;
            }
        } });

    pool.PollingCtrlSubProcess(10);

    pool.WaitSubProcess();

    cout << "所有子进程已结束, 控制进程退出" << endl;

    return 0;
}