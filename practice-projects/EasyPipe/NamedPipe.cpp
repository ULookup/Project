#include "NamedPipe.h"

#define SIZE 1024

const int default_fd = -1;

NamedPipe::NamedPipe(const std::string &pipename) : _pipename(pipename), _fd(default_fd) {}

NamedPipe::~NamedPipe() {}

bool NamedPipe::Create()
{
    if(mkfifo(_pipename.c_str(), 0666) == 0){
        std::cout << "creat namedpipe successfully" << std::endl;
        return true;
    }
    else{
        if(errno == EEXIST){
            return true;
        }
        perror("mkfifo error");
        return false;
    }
}

bool NamedPipe::Close()
{
    if(_fd == default_fd){
        perror("pipe not opened");
        return false;
    }
    else{
        close(_fd);
        _fd = default_fd;
        return true;
    }
}

bool NamedPipe::OpenForRead(bool autoConnect)
{
    Create();
    if(autoConnect)  // 自动连接模式
    {
        while(true)
        {
            _fd = open(_pipename.c_str(), O_RDONLY | O_NONBLOCK);
            if(_fd != -1){
                //成功连接后，去掉非阻塞标志
                close(_fd);
                _fd = open(_pipename.c_str(), O_RDONLY);
                if(_fd == -1){
                    perror("open for read error");
                    return false;
                }   
                return true;
            }
            else if(errno == ENXIO)
            {
                std::cout << "waiting for server ..." << std::endl;
                sleep(1);
            }
            else{
                perror("open for read error");
                return false;
            }
        }
    }
    else{            // 非自动连接模式
        _fd = open(_pipename.c_str(), O_RDONLY);
 
        if(_fd == -1){
            perror("open for read error");
            return false;
        }

        return true;
    }
}

bool NamedPipe::OpenForWrite()
{
    Create();
    _fd = open(_pipename.c_str(), O_WRONLY);

    if(_fd == -1){
        perror("open for write fail");
        return false;
    }

    return true;
}

bool NamedPipe::Read(std::string *out)
{
    if(_fd == default_fd){
        perror("pipe not opened");
        return false;
    }

    char buffer[SIZE];
    int read_bytes = read(_fd , buffer, SIZE - 1);
    if(read_bytes > 0){
        buffer[read_bytes] = '\0';
        *out = std::string(buffer);
        return true;
    }
    else if(read_bytes == 0){
        std::cout << "写端关闭, 停止读取" << std::endl;
        return false;
    }
    else{
        perror("read error");
        return false;
    }
}

bool NamedPipe::Write(const std::string &in)
{
    if(_fd == default_fd){
        perror("pipe not opened");
        return false;
    }

    int write_bytes = write(_fd, in.c_str(), in.size());
    if(write_bytes > 0){
        return true;
    }
    else{
        perror("write error");
        return false;
    }
}

void NamedPipe::Remove()
{
    unlink(_pipename.c_str());
}