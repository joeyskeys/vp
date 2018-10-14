#pragma once

#include "core/noncopyable.h"

class Tool : public NonCopyable
{
public:
    Tool();
    ~Tool();
    Tool(const Tool& b);
    Tool& operator=(const Tool& b);
    Tool(Tool&& b);
    Tool& operator=(Tool&& b);

private:

}
