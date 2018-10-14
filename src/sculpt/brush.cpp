#include "brush.h"

#include "step.h"
#include "core/context.h"

#include <utility>

Brush::Brush():
    radius(0.5f),
    falloff(0.1f),
    current_stroke(nullptr)
{

}

Brush::~Brush()
{

}

Brush::Brush(Brush&& b)
{

}

Brush& Brush::operator=(Brush&& b)
{

}

void Brush::generateNewStroke()
{
    current_stroke = new Stroke;
}

Stroke* Brush::finishCurrentStroke()
{
    Stroke *tmp = nullptr;
    std::swap(tmp, current_stroke);
    return tmp;
}

void Brush::sample(glm::vec3 info, int idx)
{
    Context &ctx = Context::instance();   
    glm::vec2 screen_pos = ctx.currentCursor();
    StepPtr = StepPtr(new Step);
    StepPtr->screen_position = screen_pos;
    if (idx != -1)
    {
        StepPtr->space_position = info;
        StepPtr->face_index = idx;
    }

    if (current_stroke)
        current_stroke->addStep(std::move(StepPtr));
}
