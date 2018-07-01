#include "camera.h"

#include <glm/gtc/type_ptr.hpp>

CameraManager* Camera::mgr = CameraManager::getInstance();

Camera::Camera():
    proj(1.0f),
    view(1.0f)
{
    void *t = mgr->getNext();
    c = new (t) CameraObj;

	c->eye = glm::vec3(0.f, 0.f, 1.f);
	c->lookat = glm::vec3(0.f, 0.f, 0.f);
}

Camera::~Camera()
{
    c->~CameraObj();
}

Camera::Camera(const Camera& rhs):
    proj(rhs.proj),
    view(rhs.view)
{
    c->eye = rhs.c->eye;
    c->lookat = rhs.c->lookat;
    c->angle = rhs.c->angle;
    c->type = rhs.c->type;
    c->near_plane = rhs.c->near_plane;
    c->far_plane = rhs.c->far_plane;
    c->fov = rhs.c->fov;
    c->ratio = rhs.c->ratio;
}

Camera& Camera::operator=(const Camera& rhs)
{
    c->eye = rhs.c->eye;
    c->lookat = rhs.c->lookat;
    c->angle = rhs.c->angle;
    c->type = rhs.c->type;
    c->near_plane = rhs.c->near_plane;
    c->far_plane = rhs.c->far_plane;
    c->fov = rhs.c->fov;
    c->ratio = rhs.c->ratio;

    return *this;
}

const glm::mat4& Camera::getProjMatrix()
{
    return proj;
}

const float* Camera::getProjMatrixPtr()
{
	return glm::value_ptr(proj);
}

const glm::mat4& Camera::getViewMatrix()
{
    return view;
}

const float* Camera::getViewMatrixPtr()
{
	return glm::value_ptr(view);
}

void Camera::translate(const glm::vec3& p)
{
	//view = glm::translate(view, p);
	c->eye += p;
	view = glm::lookAt(c->eye, c->lookat, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::rotate(const float x, const float y)
{
	glm::vec3 dir = c->lookat - c->eye;
	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 up = glm::normalize(glm::cross(right, dir));
	glm::mat4 xform = glm::rotate(glm::mat4(1.f), x, up);
	up = glm::normalize(glm::cross(dir, up));
	xform = glm::rotate(xform, y, up);
	view = xform * view;
	dir = glm::vec3(xform * glm::vec4(dir, 0.f));
	c->lookat = c->eye + dir;
}

void Camera::tilt(const float angle)
{

}
