#include "camera.h"

Camera::Camera():
    proj(1.0f),
    view(1.0f)
{
    void *t = mgr->getNext();
    c = new (t) CameraObj;
}

Camera::~Camera()
{
    c->~CameraObj;
}

Camera::Camera(const Camera& rhs):
    proj(rhs.proj),
    proj(rhs.proj)
{
    c->eye = rhs.c->eye;
    c->lookat = rhs.c->lookat;
    c->angle = rhs.c->angle;
    c->type = rhs.c->type;
    c->near = rhs.c->near;
    c->far = rhs.c->far;
    c->fov = rhs.c->fov;
    c->ratio = rhs.c->ratio;
}

Camera& operator=(const Camera& rhs)
{
    c->eye = rhs.c->eye;
    c->lookat = rhs.c->lookat;
    c->angle = rhs.c->angle;
    c->type = rhs.c->type;
    c->near = rhs.c->near;
    c->far = rhs.c->far;
    c->fov = rhs.c->fov;
    c->ratio = rhs.c->ratio;

    return *this;
}

const mat4& getProjMatrix()
{
    return proj;
}

const mat4& getViewMatrix()
{
    return view;
}

void translate(const vec3& p)
{

}

void rotate(const float x, const float y)
{

}

void tilt(const float angle)
{

}
