#include "brush.h"

#include "core/context.h"

Brush::Brush():
    radius(0.5f)
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

StepPtr Brush::sample()
{
    Context ctx = Context::instance();   
    glm::vec2 screen_pos = ctx.currentCursor();

}
