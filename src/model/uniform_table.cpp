#include "uniform_table.h"

#include <cstring>
#include <utility>

#include <rapidjson/document.h>

Uniform::Uniform(UniformType type) :
	size(size_map[static_cast<int>(type)], 
	buf(new char[size])
{

}

Uniform::~Uniform()
{

}

Uniform::Uniform(const Uniform& b) :
	size(b.size),
	buf(new char[size])
{
	memcpy(buf.get(), b.buf.get(), size);
}

Uniform& Uniform::operator=(const Uniform& b)
{

}

Uniform::(Uniform&& b) :
	size(b.size),
	buf(std::move(b.buf))
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

void Uniform::setValue(void* src)
{
	memcpy(buf.get(), src, size);
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