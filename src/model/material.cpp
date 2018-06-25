#include "material.h"

#include <fstream>
#include <utility>

#include <QOpenGLShader>
#include <QDebug>

using namespace std;

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

ShaderManager* ShaderManager::getInstance()
{
    static ShaderManager instance;
    return &instance;
}

bool ShaderManager::loadShader(const string& name)
{
    string vpath = string("G:/WorkSpace/repos/vp/src/shaders") + name + string(".vert");
    string fpath = string("G:/WorkSpace/repos/vp/src/shaders") + name + string(".frag");

    ifstream vf(vpath);
    if (!vf.good())
        return false;
    ifstream ff(fpath);
    if (!ff.good())
        return false;

    /*
    int length;
    char *buffer = nullptr;

    vf.seekg(0, vf.end);
    length = vf.tellg();
    vf.seekg(0, vf.beg);
    buffer = new char[length];
    vf.read(buffer, length);
    */
    QOpenGLShaderProgram program;
    bool ret;

    ret = program.addShaderFromSourceFile(QOpenGLShader::Vertex, vpath.c_str());
    if (!ret)
    {
        qDebug() << program.log();
        return false;
    }

    ret = program.addShaderFromSourceFile(QOpenGLShader::Fragment, fpath.c_str());
    if (!ret)
    {
        qDebug() << program.log();
        return false;
    }

    ret = program.link();
    if (!ret)
    {
        qDebug() << program.log();
        return false;
    }

    //shader_map[name] = move(program);
	return true;
}

QOpenGLShaderProgram* ShaderManager::getShader(const string& name)
{
    auto it = shader_map.find(name);
    if (it != shader_map.end())
        return it->second;
    else
        return nullptr;
}
