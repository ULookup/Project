#include "icelog/FileSink.h"

namespace icelog
{
    FileSink::~FileSink() {}

    FileSink::FileSink(const std::string &path, const std::string &filename)
        : _path(path), _filename(filename)
    {
        LockGuard lock(_lock);
        if (std::filesystem::exists(_path))
        {
            return;
        }
        try
        {
            std::filesystem::create_directories(_path);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void FileSink::SyncLog(const std::string &logmessage)
    {
        LockGuard lock(_lock);
        std::string target = _path;
        target += "/";
        target += _filename;

        std::ofstream out(target.c_str(), std::ios::app);
        if (!out.is_open())
        {
            return;
        }
        out << logmessage << std::endl;
        out.close();
    }
}