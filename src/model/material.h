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
    ~ShaderManager();
    ShaderManager(const ShaderManager& rhs);
    ShaderManager& operator=(const ShaderManager& rhs);

public:
    static ShaderManager* getInstance();
    bool loadShader(const string& name);
    QOpenGLShaderProgram* getShader(const string& name);

private:
    map<string, QOpenGLShaderProgram*> shader_map;
	vector<string> search_paths;
};
