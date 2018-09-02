#pragma once

#include "shader.h"

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <map>
#include <vector>

class ShaderManager
{
private:
	ShaderManager();

public:
    ShaderManager(const ShaderManager& rhs) = delete;
    ShaderManager& operator=(const ShaderManager& rhs) = delete;

public:
    ~ShaderManager();
    static ShaderManager* getInstance();
    bool loadShader(const string& name);
    ShaderProgram* getShader(const string& name);

private:
    std::map<string,ShaderProgram> shader_map;
    std::vector<string> search_paths;
};
