#pragma once

// code from appleseed

#include "noncopyable.h"

template <typename T>
class Singleton : public NonCopyable
{
public:
    typedef T ObjectType;

    // Return the unique instance of this class.
    static ObjectType& instance()
    {
        static ObjectType object;
        return object;
    }

protected:
    // Allow derived class to have a constructor and a destructor.
    Singleton() {}
    virtual ~Singleton() {}
};
