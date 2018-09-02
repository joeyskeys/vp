#pragma once

#include <glm/vec3.hpp>

#include "manager.h"

typedef struct LightObj
{
    char type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;
    float angle;
}LightObj;

#define LIGHT_PNT 1
#define LIGHT_DIR 2
#define LIGHT_SPT 4

class Light
{
public:
    Light();
    ~Light();
    Light(const Light& rhs);
    Light& operator=(const Light& rhs);

    void    read(const Light *l);
    void    write(Light *l);
    void    setType(const char t);
    void    setPosition(const glm::vec3& pos);
    void    setColor(const glm::vec3& col);
	void    setDirection(const glm::vec3& dir);
    void    setAngle(const float a);

    void*   getData() const;
    int     getSize() const;

private:
    LightObj *l;

    static LightManager *mgr;
};
