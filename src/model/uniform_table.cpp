#include "uniform_table.h"

#include <cstring>
#include <utility>

#include <rapidjson/document.h>

Uniform::Uniform(UniformType type):
	size(size_map[static_cast<int>(type)], 
	buf(new char[size]),
    location(-1),
    update_value_func(update_func_map[static_cast<int>(type)]
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
    *this(b);
    return *this;
}

Uniform::(Uniform&& b):
	size(b.size),
	buf(std::move(b.buf)),
    location(b.location),
    update_value_func(b.update_value_func)
{

}

Uniform& operator=(Uniform&& b)
{
	*this(b);
	return *this;
}

template <typename T>
T* Uniform::getPtr()
{
	return static_cast<T*>(buf.get());
}

UniformTable::UniformTable()
{

}

UniformTable::~UniformTable()
{

}

UniformTable::UniformTable(UniformTable&& b):
	uniform_map(b.uniform_map)
{

}

UniformTable& UniformTable::operator=(UniformTable&& b)
{
	*this(b);
	return *this;
}

bool UniformTable::loadDescription(std::string& filepath)
{
	AutoBuffer buf = readAll(filepath);
	rapidjson::Document doc;
	doc.Parse(buf.get());
	if (doc.HasParseError())
		return false;

	for (rapidjson::Value::ConstMemberIterator it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
	{
		uniform_map[it->name.GetString()] = Uniform(static_cast<UniformType>(it->value.GetInt()));
	}

	return true;
}

void UniformTable::updateLocation(ShaderProgram *p)
{
    GLuint proj_id = p->getProgram();
    for (UniformMap::iterator it; it != uniform_map.end(); it++)
    {
        GLint loc = glGetUniformLocation(proj_id, it->first.c_str());
        if (loc == -1)
            continue;

        it->second.setLocation(loc);
    }
}

void UniformTable::updateUniform(std::string& name, void* data)
{
	UniformMap::iterator it = uniform_map.find(name);
	if (it != uniform_map.end())
		it->second.setValue(data);
}

void UniformTable::uploadUniforms()
{
    for (UniformMap::iterator it = uniform_map.begin(); it != uiform_map.end(); ++it)
        it->second.upload();
}
