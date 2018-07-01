#include "manager.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace glm;

typedef struct CameraObj
{
    glm::vec3 eye;
    glm::vec3 lookat;
    float angle;

    char type;
	float near_plane;
	float far_plane;
    union {
        float fov;
        float width;
	};
    union {
        float ratio;
        float height;
	};
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

    const glm::mat4& getProjMatrix();
	const float*	 getProjMatrixPtr();
    const glm::mat4& getViewMatrix();
	const float*	 getViewMatrixPtr();

    void translate(const glm::vec3& p);
    void rotate(const float x, const float y);
    void tilt(const float angle);

private:
    CameraObj *c;
    glm::mat4 proj;
    glm::mat4 view;

    static CameraManager *mgr;
};
