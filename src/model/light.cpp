#include "light.h"

#include <utility>

LightManager* Light::mgr = LightManager::getInstance();

Light::Light()
{
    void *t = mgr->getNext();
    l = new (t) LightObj;
}

Light::~Light()
{
    l->~LightObj();
}

Light::Light(const Light& rhs)
{
    l->type = rhs.l->type;
    l->position = rhs.l->position;
    l->color = rhs.l->color;
    l->direction = rhs.l->direction;
    l->angle = rhs.l->angle;
};

Light& Light::operator=(const Light& rhs)
{
    l->type = rhs.l->type;
    l->position = rhs.l->position;
    l->color = rhs.l->color;
    l->direction = rhs.l->direction;
    l->angle = rhs.l->angle;

    return *this;
}

void Light::setType(const char t)
{
    l->type = t;
}

void Light::setPosition(const glm::vec3& pos)
{
    l->position = pos;
}

void Light::setColor(const glm::vec3& col)
{
    l->color = col;
}

void Light::setDirection(const glm::vec3& dir)
{
    l->direction = dir;
}

void Light::setAngle(const float a)
{
    l->angle = a;
}

void* Light::getData() const
{
	if (l->type == LIGHT_DIR)
		return &(l->color);
	else
		return &(l->position);
}

int Light::getSize() const
{
    if (l->type == LIGHT_SPT)
		return sizeof(glm::vec3) * 3 + sizeof(float);
    else
		return sizeof(glm::vec3) * 2;
}
