#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;


class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task &&cb);
    Task getTask();

private:
    void threadFunc();

private:
    size_t  _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;

};

#endif
