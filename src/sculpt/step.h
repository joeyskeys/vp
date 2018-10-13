#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <memory>

class Step
{
public:
    Step();
    ~Step();
    Step(const Step& b);
    Step& operator=(const Step& b);
    Step(Step&& b);
    Step& operator=(Step&& b);

private:
    float intensity;
    glm::vec2 screen_position;
    glm::vec3 space_position;
    glm::vec3 normal;
};

using StepPtr = std::unique_ptr<Step>;