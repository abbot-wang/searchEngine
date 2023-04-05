#include "../include/Condition.h"
#include "../include/MutexLock.h"

Condition::Condition(MutexLock &mutex)
: _mutex(mutex)
{
    pthread_cond_init(&_cond, nullptr);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
    pthread_cond_wait(&_cond, _mutex.getMuextLockPtr());//_mutex
}

void Condition::notify()
{
    pthread_cond_signal(&_cond);
}

void Condition::notifyAll()
{
    pthread_cond_broadcast(&_cond);
}


