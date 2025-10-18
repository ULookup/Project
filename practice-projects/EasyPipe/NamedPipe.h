#ifndef __NAMEDPIPE_H__
#define __NAMEDPIPE_H__

#include "common.hpp"

class NamedPipe
{
public:
    NamedPipe(const std::string &pipename);
    ~NamedPipe();

    bool Create();
    bool Close();
    bool OpenForRead(bool autoConnect = true);
    bool OpenForWrite();
    bool Read(std::string *out);
    bool Write(const std::string &in);

    void Remove();
private:
    std::string _pipename;
    int _fd;
};

#endif