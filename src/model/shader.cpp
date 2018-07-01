#include "shader.h"

#include <iostream>
#include <fstream>
#include <cstdio>
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
        glDeleteShader(program);
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

static size_t getFileSize(ifstream& f)
{
    f.seekg(0, ios::beg);
    streampos pos = f.tellg();
    f.seekg(0, ios::end);
	size_t size = f.tellg() - pos;
	f.seekg(0, ios::beg);
    return size;
}

static GLuint loadShader(GLenum type, char *buf, GLint size)
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
		cout << log << endl;
        free(log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ShaderProgram::load(const string& p, const string& n)
{
    string vpath = p + n + ".vert";
    ifstream vf(vpath, ios::in | ios::binary);
    if (!vf.good())
        return false;

    string fpath = p + n + ".frag";
    ifstream ff(fpath, ios::in | ios::binary);
    if (!ff.good())
        return false;

    char    *buf;
    size_t  size;

    size = getFileSize(vf);
    buf = (char*)malloc(size + 1);
    vf.read(buf, size);
	buf[size] = 0;
	//FILE *vff = fopen(vpath.c_str(), "r");
	//fread(buf, 1, size, vff);
	//fclose(vff);
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
        return false;

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
        //cerr << buf << endl;
		cout << buf << endl;
        free(buf);
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    return true;
}

