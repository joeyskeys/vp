#include <glm/vec3.hpp>
#include <glm/mat4.hpp>

using namespace glm;

typedef struct CameraObj
{
    vec3 eye;
    vec3 lookat;
    float angle;

    char type;
    float near;
    float far;
    union {
        float fov;
        float width;
    }
    union {
        float ratio;
        float height;
    }
} CameraObj;

#define CAMERA_PERSP 0
#define CAMERA_ORTHO 1

class Camera
{
public:
    Camera();
    ~Camera();
    Camera(const Camera& rhs);
    Camera& operator=(const Camera& rhs);

    const mat4& getProjMatrix();
    const mat4& getViewMatrix();

    void translate(const vec3& p);
    void rotate(const float x, const float y);
    void tilt(const float angle);

private:
    CameraObj *c;
    mat4 proj;
    mat4 view;

    static const CameraManager *mgr = CameraManager.getInstance();
};
