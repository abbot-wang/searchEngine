#ifndef  __NOCOPTABLE_H__
#define __NOCOPTABLE_H__

class NoCopyable
{
public:
protected:
/* private: */
    NoCopyable()
    {

    }
    ~NoCopyable()
    {

    }

    NoCopyable(const NoCopyable &rhs) = delete;
    NoCopyable &operator=(const NoCopyable &rhs) = delete;


};

#endif
