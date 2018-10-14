#include "stroke.h"

#include <utility>

Stroke::Stroke()
{

}

Stroke::~Stroke()
{

}

Stroke::Stroke(Stroke&& b):
    steps(std::move(b.steps))
{

}

Stroke& Stroke::operator=(Stroke&& b)
{
    steps = std::move(b.steps);
}

void Stroke::addStep(StepPtr&& step_ptr)
{
    steps.emplace_back(step_ptr);
}
