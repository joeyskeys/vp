#include "camera.h"

#include <glm/gtc/type_ptr.hpp>

CameraManager* Camera::mgr = CameraManager::getInstance();

Camera::Camera():
    proj(1.0f),
    view(1.0f)
{
    void *t = mgr->getNext();
    c = new (t) CameraObj;

	c->eye = glm::vec3(0.f, 0.f, 2.f);
	c->lookat = glm::vec3(0.f, 0.f, -1.f);
	c->fov = 1.047f;
	c->near_plane = 1.f;
	c->ratio = 4.f / 3.f;

	view = glm::lookAt(c->eye, c->lookat, glm::vec3(0.f, 1.f, 0.f));
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

void Camera::translateAroundPivot(const float x, const float y)
{
	float distance = glm::length(c->lookat - c->eye);
	float ratio = distance / c->near_plane;
	float film_width = glm::tan(c->fov / 2.f) * c->near_plane * 2;
	float film_height = film_width / c->ratio;
	//float rx = x * film_width;
	//float ry = -y * film_height;
	float rx = x * ratio * 1.5;
	float ry = -y * ratio / c->ratio * 1.5;
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 dir = c->lookat - c->eye;
	glm::vec3 right = glm::normalize(glm::cross(dir, up));
	up = glm::normalize(glm::cross(right, dir));
	c->eye -= up * ry;
	c->eye -= right * rx;
	c->lookat = c->eye + dir;
	view = glm::lookAt(c->eye, c->lookat, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::rotate(const float x, const float y)
{
	float rx = x / 180.f;
	float ry = y / 180.f;
	glm::vec3 dir = c->lookat - c->eye;
	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.f, 1.f, 0.f)));
	glm::mat4 xform = glm::rotate(glm::mat4(1.f), -rx, glm::vec3(0.f, 1.f, 0.f));
	xform = glm::rotate(xform, -ry, right);
	view = xform * view;
	dir = glm::vec3(xform * glm::vec4(dir, 0.f));
	c->lookat = c->eye + dir;
}

void Camera::rotateAroundPivot(const float x, const float y)
{
	float rx = x / 180.f;
	float ry = y / 180.f;
	glm::vec3 rev_dir = c->eye - c->lookat;
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 xform = glm::rotate(glm::mat4(1.f), -rx, up);
	glm::vec3 right = glm::normalize(glm::cross(rev_dir, up));
	xform = glm::rotate(xform, ry, right);
	c->eye = glm::vec3(glm::vec4(c->lookat, 1.f) + xform * glm::vec4(rev_dir, 0.f));
	view = glm::lookAt(c->eye, c->lookat, up);
}

void Camera::zoom(const float v)
{
	glm::vec3 rev_dir = c->eye - c->lookat;
	c->eye = c->lookat + (v + 1.f) * rev_dir;
	view = glm::lookAt(c->eye, c->lookat, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::tilt(const float angle)
{

}
