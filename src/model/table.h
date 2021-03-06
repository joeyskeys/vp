#pragma once

#include "utils.h"

#include <rapidjson/document.h>
#include <GL/glew.h>
#include <map>
#include <string>
#include <utility>
#include <cstring>
#include <iostream>

enum class UniformType
{
    INT = 0,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4
};

const static int size_map[] = {4, 4, 8, 12, 16, 36, 64};

class Uniform;
class ShaderProgram;

using UniformPair = std::pair<int, void*>;
using UniformMap = std::map<std::string, Uniform>;
using UpdateFunction = void (*)(int, void*);
using AttribMap = std::map<int, int>;

const static UpdateFunction update_func_map[] = {
    [](int loc, void* data) { glUniform1iv(loc, 1, (GLint*)(data)); },
    [](int loc, void* data) { glUniform1fv(loc, 1, (GLfloat*)(data)); },
    [](int loc, void* data) { glUniform2fv(loc, 1, (GLfloat*)(data)); },
    [](int loc, void* data) { glUniform3fv(loc, 1, (GLfloat*)(data)); },
    [](int loc, void* data) { glUniform4fv(loc, 1, (GLfloat*)(data)); },
    [](int loc, void* data) { glUniformMatrix3fv(loc, 1, GL_FALSE, (GLfloat*)(data)); },
    [](int loc, void* data) { glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)(data)); }
};

class Uniform
{
public:
    Uniform(UniformType type);
    ~Uniform();
    Uniform(const Uniform& b);
    Uniform& operator=(const Uniform& b);
    Uniform(Uniform&& b);
    Uniform& operator=(Uniform&& b);

    template <typename T>
    T* getPtr();

    inline void setValue(const void* src) { memcpy(buf.get(), src, size); }
    inline GLint getLocation() { return location; }
    inline void setLocation(GLint loc) { location = loc; }
    inline void upload() { update_value_func(location, buf.get()); }

private:
    size_t  size;
    AutoBuffer buf;
    GLint   location;
    UpdateFunction update_value_func;
};

class UniformTable: public UniformMap
{
public:
    UniformTable();
    virtual ~UniformTable();
    UniformTable(const UniformTable& b) = delete;
    UniformTable& operator=(const UniformTable& b) = delete;
    //UniformTable(UniformTable&& b);
    //UniformTable& operator=(UniformTable&& b);

    bool loadDescription(const std::string& path);
    bool loadDescription(const rapidjson::Value& v);
    void updateLocation(const ShaderProgram *p);
    void updateUniform(const std::string& name, const void* data);
    void uploadUniforms();

//private:
//    UniformMap uniform_map;
};

class AttribTable: public AttribMap
{
public:
    AttribTable();
    virtual ~AttribTable();
    AttribTable(const AttribTable& b) = delete;
    AttribTable& operator=(const AttribTable& b) = delete;
    //AttribTable(AttribTable&& b);
    //AttribTable& operator=(AttribTable&& b);

    bool loadDescription(const rapidjson::Value& v);

//private:
//    AttribMap attrib_map;
};

bool loadDescriptionFile(std::string& path, UniformTable& ut, AttribTable& at);
