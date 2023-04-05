#include "../include/ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
        _isExit = true;
    }
}

//线程池开始执行的时候，其实就是工作线程已经开启
void ThreadPool::start()
{
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
       /* unique_ptr<Thread> up(new WorkThread(*this)); */
       unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this))) ;
       _threads.push_back(std::move(up));
    }

    for(auto &th : _threads)
    {
        th->start();//创建工作线程的id，将工作线程开始运行
    }
}

//等同于在生产者消费者例子里面的生产者
void ThreadPool::addTask(Task &&task)
{
    if(task)
    {
        _taskQue.push(std::move(task));
    }
}

Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

//线程池的退出，其实就是线程池中几个工作线程的退出，而工作线程
//WorkThread就是从Thread继承过来的，所以每个工作线程执行join
//方法就可以了
void ThreadPool::stop()
{
    //只要任务队列中有数据，线程池中的工作线程就不能退出，让其
    //sleep
    while(!_taskQue.empty())
    {
        sleep(1);
    }

    _isExit = true;
    //将所有等在在_notEmpty上的工作线程全部唤醒
    _taskQue.wakeup();
    for(auto &th : _threads)
    {
        th->join();
    }

}

//在线程池中封装的任务，这个任务的实际执行者WorkThread
void ThreadPool::threadFunc()
{
    //只要线程池没有退出，就可以一直获取任务，并且执行
    //相应的process
    while(!_isExit)
    {
        //getTask如果执行的非常慢的话，线程池的stop有可能先执行，会将
        //_isExit设置为true
        //但是如果getTask执行的过快的话，来不及将_isExit设置为true，
        //一直卡在getTask，也就是卡在Condition中的wait上面
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb();
        }

    }
}
