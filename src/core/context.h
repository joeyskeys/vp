#pragma once

#include "singleton.h"
#include "model/mesh.h"
#include "model/dynamic_mesh.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Context : public Singleton<Context>
{
public:
    Mesh current_mesh;
    DynamicMesh current_dynamic_mesh;
    
    glm::vec2 currentCursor();
};
