#include "myshell.h"

#define SIZE 1024

int main()
{
  char commandstr[SIZE];
  Begin();
  while(true)
  {
     //0.初始化全局变量
     InitGlobal();
     //1.输出命令行提示符
     PrintCommandPromot();
     //2.获取用户输入的命令
     if(!GetCommandString(commandstr,SIZE))
       continue;
     //3.检测命令是不是重定向
     CheckRedir(commandstr);
     //4.对输入的命令行进行解析
     ParseCommandString(commandstr);
    //5.检测命令，内建命令要让shell自己执行
    if(BuildInCommandExec()){ continue; }
    //6.执行命令，让子进程来执行
    ForkAndExec();
  }
}
