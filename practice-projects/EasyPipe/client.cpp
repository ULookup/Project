#include "common.hpp"
#include "NamedPipe.h"

int main()
{
    //创建命名管道类（文件名和server端保持一致）
    NamedPipe pipe(PIPENAME);

    //打开命名管道文件
    pipe.OpenForWrite();

    //轮询发送消息
    while(true)
    {
        std::cout << "Please enter #";
        std::string message;
        std::getline(std::cin, message);
        if(message == "exit"){
            break;
        }
        pipe.Write(message);
    }

    //关闭命名管道文件描述符
    pipe.Close();

    return 0;
}