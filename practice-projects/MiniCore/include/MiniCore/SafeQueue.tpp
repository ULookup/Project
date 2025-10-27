#include "MiniCore/SafeQueue.h"

namespace MiniCore
{
    template<class T>
    SafeQueue<T>::SafeQueue(size_t capacity) : 
        _buffer(capacity),
        _capacity(capacity), 
        _producter(0), 
        _consumer(0),
        _count(0)
    {}

    template<class T>
    void SafeQueue<T>::push(const T& item)
    {
        _lock.lock();
        while(_count == _capacity){
            _not_full.Wait(_lock);
        }

        _buffer[_producter] = item;
        _producter = ( _producter + 1 ) % _capacity;
        ++_count;

        _lock.unlock();
        _not_empty.NotifyOne();
    }

    template<class T>
    bool SafeQueue<T>::pop(T& item)
    {
        _lock.lock();
        while(_count == 0){
            _not_empty.Wait(_lock);
        }

        item = _buffer[_consumer];
        _consumer = ( _consumer + 1 ) % _capacity;
        --_count;

        _lock.unlock();
        _not_full.NotifyOne();

        return true;
    }

    template<class T>
    bool SafeQueue<T>::empty() const
    {
        _lock.lock();
        int ret = ( _count == 0 );
        _lock.unlock();
        return ret;
    }

    template<class T>
    bool SafeQueue<T>::full() const
    {
        _lock.lock();
        int ret = ( _count == _capacity );
        _lock.unlock();
        return ret;
    }
}