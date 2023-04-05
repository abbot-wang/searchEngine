#include "../include/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(Elem  &&value)
{
    MutexLockGuard autoLock(_mutex);//autoLock栈对象

    while(full())//虚假唤醒
    {
        _notFull.wait();
    }

    _que.push(std::move(value));//队列中就有数据

    _notEmpty.notify();


}

Elem TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);//autoLock栈对象

    while(_flag && empty())
    {
        _notEmpty.wait();//上半部与下半部
    }

    if(_flag)
    {
        Elem tmp = _que.front();
        _que.pop();

        _notFull.notify();

        return tmp;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}
