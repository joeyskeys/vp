#include "step.h"

#include "core/context.h"

#include <utility>

Step::Step():
    intensity(1.f),
    screen_position(0.f),
    space_position(0.f),
    normal(0.f)
{

}

Step::~Step()
{

}

Step::Step(const Step& b):
    intensity(b.intensity),
    screen_position(b.screen_position),
    space_position(b.space_position),
    normal(b.normal)
{

}

Step& operator=(const Step& b)
{
    intensity = b.intensity;
    screen_position = b.screen_position;
    space_position = b.space_position;
    normal = b.normal;
}

Step::Step(Step&& b):
    intensity(b.intensity),
    screen_position(std::move(b.screen_position)),
    space_position(std::move(b.space_position)),
    normal(std::move(b.normal))
{

}

Step& operator=(Step&& b)
{
    intensity = b.intensity;
    screen_position = std::move(b.screen_position);
    space_position = std::move(b.space_position);
    normal = std::move(b.normal)
}

void Step::apply()
{
    Context &ctx = Context::instance();

}
