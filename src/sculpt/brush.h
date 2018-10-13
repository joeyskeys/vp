#pragma once

#include "step.h"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>

class Brush
{
public:
    Brush();
    ~Brush();
    Brush(const Brush& b) = delete;
    Brush& operator=(const Brush& b) = delete;
    Brush(Brush&& b);
    Brush& operator=(Brush&& b);

    StepPtr sample(); 

private:
    float radius;
    float falloff;
};
