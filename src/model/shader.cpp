#include "shader.h"

#include <iostream>
#include <fstream>
#include <vector>

ShaderProgramManager* ShaderProgram::mgr = ShaderProgramManager::getInstance();

ShaderProgram::ShaderProgram():
    program(0),
    initailized(false)
{
    void *t = mgr->getNext();
    s = new (t) ShaderProgramObj;
}

ShaderProgram::~ShaderProgram()
{
    if (program)
        glDeleteShader(program);
    s->~ShaderProgramObj();
}

static size_t getFileSize(const ifstream& f)
{
    f.seekg(0, ios::beg);
    size_t pos = f.tellg();
    f.seekg(0, ios::end);
    return f.tellg() - pos;
}

static GLuint loadShader(GLenum type, char *buf, size_t size)
{
    GLint succ, logsize;
    char* log;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &buf, &size);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);
    if (succ == GL_FALSE)
    {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
        buf = (char*)malloc(logsize);
        glGetShaderInfoLog(shader, logsize, &logsize, log);
        cerr << log;
        free(buf);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ShaderProgram::load(const string& p, const string& n)
{
    string vpath = p + n + ".vert";
    ifstream vf(vpath);
    if (!vf.good())
        return false;

    string fpath = p + n + ".frag";
    ifstream ff(fpath);
    if (!ff.good())
        return false;

    char    *buf;
    size_t  size;

    size = getFileSize(vf);
    buf = (char*)malloc(size);
    vf.read(buf, size);
    GLuint vs = loadShader(GL_VERTEX_SHADER, buf, size);
    if (!vs)
        return false;
    free(buf);

    size = getFileSize(ff);
    buf = (char*)malloc(size);
    GLuint fs = loadShader(GL_FRAGMENT_SHADER, buf, size);
    if (!fs)
        return false;
    free(buf);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint succ;
    glGetProgramiv(program, GL_LINK_STATUS, &succ);
    if (succ == GL_FALSE)
    {
        GLint logsize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
        buf = (char*)malloc(logsize);
        cerr << buf << endl;
        free(buf);
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    return true;
}

