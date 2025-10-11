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
     //3.对输入的命令行进行解析
     ParseCommandString(commandstr);

    //4.检测命令，内建命令要让shell自己执行
    if(BuildInCommandExec()){ continue; }
    //5.执行命令，让子进程来执行
    ForkAndExec();
  }
}
