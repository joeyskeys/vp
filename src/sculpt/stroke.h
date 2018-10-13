#pragma once

#include "step.h"

#include <vector>

class Stroke
{
public:
    Stroke();
    ~Stroke();
    Stroke(const Stroke& b) = delete;
    Stroke& operator=(const Stroke& b) = delete;
    Stroke(Stroke&& b);
    Stroke& operator=(Stroke&& b);

private:
    std::vector<Step*> steps
};
