#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__


//不允许复制或者赋值
class NonCopyable
{
protected:
    NonCopyable()
    {
    }
    ~NonCopyable()
    {

    }

    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;


};

#endif
