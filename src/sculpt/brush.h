#pragma once

#include "step.h"
#include "stroke.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class Brush
{
public:
    Brush();
    ~Brush();
    Brush(const Brush& b) = delete;
    Brush& operator=(const Brush& b) = delete;
    Brush(Brush&& b);
    Brush& operator=(Brush&& b);

    void    generateNewStroke();
    Stroke* finishCurrentStroke();
    void    sample(glm::vec3 info, int idx); 

    void    applyToMesh();

private:
    float radius;
    float falloff;
    Stroke *current_stroke
};
