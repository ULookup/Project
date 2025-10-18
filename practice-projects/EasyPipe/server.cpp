#include "common.hpp"
#include "NamedPipe.h"

int main()
{
    //创建命名管道类
    NamedPipe pipe(PIPENAME);

    //调用系统调用在磁盘创建命名管道文件
    //打开命名管道文件
    pipe.OpenForRead(true);

    //创建用于接收消息的字符串
    std::string message;

    ///轮询读取消息
    while(true)
    {
        message[0] = '\0';
        bool ret = pipe.Read(&message);
        if(!ret){
            break;
        }
        std::cout << "Received message: " << message << std::endl;
    }

    //关闭命名管道文件描述符
    pipe.Close();

    //删除命名管道文件
    pipe.Remove();

    return 0;
}