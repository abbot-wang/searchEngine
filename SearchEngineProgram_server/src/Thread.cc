#include "../include/Thread.h"
#include <stdio.h>

Thread::Thread(ThreadCallback &&cb)
: _thid(0)
, _isRunning(false)
, _cb(std::move(cb))
{
}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_thid);
    }
}

//unique_ptr<Thread> pthread(new MyThread());
void Thread::start()//this
{
    //shift + k
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }

    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread *pth = static_cast<Thread *>(arg);
    if(pth)
    {
        pth->_cb();//回调函数
    }


    //return nullptr
    pthread_exit(nullptr);
}
