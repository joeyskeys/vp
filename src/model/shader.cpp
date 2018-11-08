#include "shader.h"
#include "utils.h"
#include "table.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

ShaderProgramManager* ShaderProgram::mgr = ShaderProgramManager::getInstance();

ShaderProgram::ShaderProgram():
    program(0)
{
    void *t = mgr->getNext();
    s = new (t) ShaderProgramObj;
}

ShaderProgram::~ShaderProgram()
{
    if (program)
        glDeleteProgram(program);
    s->~ShaderProgramObj();
}

ShaderProgram::ShaderProgram(ShaderProgram&& rhs)
{
	s = rhs.s;
	rhs.s = nullptr;
	program = rhs.program;
	rhs.program = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs)
{
	s = rhs.s;
	rhs.s = nullptr;
	program = rhs.program;
	rhs.program = 0;

	return *this;
}

static GLuint loadShader(GLenum type, const char *buf, GLint size)
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
        log = (char*)malloc(logsize);
        glGetShaderInfoLog(shader, logsize, &logsize, log);
        //cerr << log << endl;
        std::cout << log << endl;
        free(log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ShaderProgram::load(const std::string& p, const std::string& n)
{
    std::string vpath = p + n + ".vert";
    ifstream vf(vpath, std::ios::in | std::ios::binary);
    if (!vf.good())
        return false;

    string fpath = p + n + ".frag";
    ifstream ff(fpath, std::ios::in | std::ios::binary);
    if (!ff.good())
        return false;
        
    string upath = p + n + ".json";
    ifstream uf(upath, ios::in | ios::binary);
    if (!uf.good())
        return false;

    bool has_geometry_shader = false;
    std::string gpath = p + n + ".geom";
    ifstream gf(gpath, std::ios::in | std::ios::binary);
    if (gf.good())
        has_geometry_shader = true;

    //if (!uniform_table.loadDescription(upath))
    if (!loadDescriptionFile(upath, uniform_table, attrib_table))
        return false;

    char    *buf;
    size_t  size;

    size = getFileSize(vf);
    buf = (char*)malloc(size + 1);
    vf.read(buf, size);
	buf[size] = 0;

	vf.close();
    GLuint vs = loadShader(GL_VERTEX_SHADER, buf, size + 1);
    free(buf);
    if (!vs)
        return false;

    size = getFileSize(ff);
    buf = (char*)malloc(size + 1);
	ff.read(buf, size);
	buf[size] = 0;
	ff.close();
    GLuint fs = loadShader(GL_FRAGMENT_SHADER, buf, size + 1);
	free(buf);
    if (!fs)
    {
        glDeleteShader(vs);
        return false;
    }

    GLuint gs = 0;
    if (has_geometry_shader)
    {
        std::cout << "init geom shader" << std::endl;
        size = getFileSize(gf);
        buf = (char*)malloc(size + 1);
        gf.read(buf, size);
        buf[size] = 0;
        gf.close();
        gs = loadShader(GL_GEOMETRY_SHADER, buf, size + 1);
        free(buf);
        if (!gs)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            return false;
        }
    }

    std::cout << "make program" << std::endl;
    if (program)
        glDeleteProgram(program);
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    if (has_geometry_shader)
        glAttachShader(program, gs);
    glLinkProgram(program);
    GLint succ;
    glGetProgramiv(program, GL_LINK_STATUS, &succ);

    glDeleteShader(vs);
    glDeleteShader(fs);
    if (has_geometry_shader)
        glDeleteShader(gs);

    if (succ == GL_FALSE)
    {
        GLint logsize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
        buf = (char*)malloc(logsize);
        glGetProgramInfoLog(program, logsize, &logsize, buf);
		cout << buf << endl;
        free(buf);
        glDeleteProgram(program);
        program = 0;
        return false;
    }

    return true;
}

