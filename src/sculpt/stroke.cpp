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
