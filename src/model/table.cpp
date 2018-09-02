#include "uniform_table.h"
#include "shader.h"

#include <iostream>
#include <cstring>
#include <utility>

#include <rapidjson/document.h>

Uniform::Uniform(UniformType type):
    size(size_map[static_cast<int>(type)]), 
    buf(new char[size]),
    location(-1),
    update_value_func(update_func_map[static_cast<int>(type)])
{

}

Uniform::~Uniform()
{

}

Uniform::Uniform(const Uniform& b):
    size(b.size),
    buf(new char[size]),
    location(b.location),
    update_value_func(b.update_value_func)
{
    memcpy(buf.get(), b.buf.get(), size);
}

Uniform& Uniform::operator=(const Uniform& b)
{
    size = b.size;
    buf = AutoBuffer(new char[size]);
    location = b.location;
    update_value_func = b.update_value_func;
    return *this;
}

Uniform::Uniform(Uniform&& b):
    size(b.size),
    buf(std::move(b.buf)),
    location(b.location),
    update_value_func(b.update_value_func)
{

}

Uniform& Uniform::operator=(Uniform&& b)
{
    size = b.size;
    buf = std::move(b.buf);
    location = b.location;
    update_value_func = b.update_value_func;
    return *this;
}

template <typename T>
T* Uniform::getPtr()
{
    return static_cast<T*>(buf.get());
}

UniformTable::UniformTable():
    UniformMap()
{

}

UniformTable::~UniformTable()
{

}

/*
UniformTable::UniformTable(UniformTable&& b):
    uniform_map(b.uniform_map)
{

}

UniformTable& UniformTable::operator=(UniformTable&& b)
{
    uniform_map = std::move(b.uniform_map);
    return *this;
}
*/

bool UniformTable::loadDescription(const std::string& filepath)
{
    AutoBuffer buf = readAll(filepath);
    rapidjson::Document doc;
    doc.Parse(buf.get());
    if (doc.HasParseError())
        return false;

    for (rapidjson::Value::ConstMemberIterator it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
    {
        this->emplace(it->name.GetString(), Uniform(static_cast<UniformType>(it->value.GetInt())));
    }

    return true;
}

void UniformTable::updateLocation(const ShaderProgram *p)
{
    GLuint proj_id = p->getProgram();
    for (UniformMap::iterator it = this->begin(); it != this->end(); ++it)
    {
        GLint loc = glGetUniformLocation(proj_id, it->first.c_str());
        if (loc == -1)
            continue;

        it->second.setLocation(loc);
    }
}

void UniformTable::updateUniform(const std::string& name, const void* data)
{
    UniformMap::iterator it = this->find(name);
    if (it != this->end())
    {
        it->second.setValue(data);
    }
}

void UniformTable::uploadUniforms()
{
    for (UniformMap::iterator it = this->begin(); it != this->end(); ++it)
        it->second.upload();
}

AttribTable::AttribTable():
    AttribMap()
{

}

AttribTable::~AttribTable()
{

}

bool AttribTable::loadDescription(rapidjson::Value& v)
{

}
