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

    inline void addStep(StepPtr&& step_ptr) { steps.emplace_back(step_ptr); }

private:
    std::vector<StepPtr> steps
};
