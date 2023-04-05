#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>

using std::function;
using std::bind;

using ThreadCallback = function<void()>;

class Thread
{
public:
    Thread(ThreadCallback &&cb);
    ~Thread();
    void start();
    void join();

private:
    //线程入口函数
    static void *threadFunc(void *arg);

private:
    pthread_t _thid;
    bool _isRunning;
    //要去实现的任务
    ThreadCallback _cb;

};

#endif
