#include <glm/vec3.hpp>

using namespace glm;

typedef struct PointLightObj
{
    vec3 position;
    vec3 color;
} PointLightObj;

typedef struct DirectionalLightObj
{
    vec3 position;
    vec3 direction;
    vec3 color;
} DirectionalLightObj;

typedef struct SpotLightObj
{
    vec3 position;
    vec3 direction;
    vec3 angle;
    vec3 color;
} SpotLightObj;

class Light
{
public:
    void setPosision(vec3& pos) = 0;
    void setColor(vec3& color) = 0;
}
