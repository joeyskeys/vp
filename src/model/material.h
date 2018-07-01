#pragma once

#include "shader.h"

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <map>

using namespace std;

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
    map<string,ShaderProgram> shader_map;
	vector<string> search_paths;
};
