#pragma once

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
    bool loadShader(string& name);
    QOpenGLShaderProgram* getShader(string& name);

private:
    map<string, QOpenGLShaderProgram> shader_map;
};
