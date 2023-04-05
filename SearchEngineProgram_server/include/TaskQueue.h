#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Task.h"
#include "Condition.h"
#include <queue>

using std::queue;

using Elem = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(Elem &&value);
    Elem pop();

    //将所有的等在在_notEmpty上的线程唤醒
    void wakeup();

private:
    size_t _queSize;
    queue<Elem> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;

};

#endif
