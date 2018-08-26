#include "utils.h"

#include <GL/glew.h>
#include <map>
#include <string>
#include <utility>

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

const static int size_map[] = {4, 4, 8, 12, 16, 36, 48};

class Uniform;
class ShaderProgram;

using UniformPair = std::pair<int, void*>;
using UniformMap = std::map<std::string, Uniform>;
using UpdateFunction = void (*)(int, void*);
using UpdateMemberFunc = void (*)(void*);

const static UpdateFunction update_func_map[] = {
    [](int loc, void* data) { glUniform1iv(loc, 1, static_cast<GLint*>(data)); }
    [](int loc, void* data) { glUniform1fv(loc, 1, static_cast<GLfloat*>(data)); }
    [](int loc, void* data) { glUniform2fv(loc, 1, static_cast<GLfloat*>(data)); }
    [](int loc, void* data) { glUniform3fv(loc, 1, static_cast<GLfloat*>(data)); }
    [](int loc, void* data) { glUniform4fv(loc, 1, static_cast<GLfloat*>(data)); }
    [](int loc, void* data) { glUniformMatrix3fv(loc, 1, GL_FALSE, static_cast<GLfloat*>(data)); }
    [](int loc, void* data) { glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<GLfloat*>(data)); }
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

	inline void setValue(void* src) { memcpy(buf.get(), src, size); }
    inline GLint getLocation() { return location; }
    inline void setLocation(GLint loc) { location = loc; }
    inline UniformType getType() { return type; }
    inline void upload() { update_value_func(location, buf.get()); }

private:
	size_t  size;
	AutoBuffer buf;
    GLint   location;
    UpdateMemberFunc update_value_func;
}

class UniformTable
{
public:
	UniformTable();
	~UniformTable();
	UniformTable(const UniformTable& b) = delete;
	UniformTable& operator=(const UniformTable& b) = delete;
	UniformTable(UniformTable&& b);
	UniformTable& operator=(UniformTable&& b);

	bool loadDescription(std::string& filepath);
    void updateLocation(ShaderProgram *p);
    void uploadUniforms();

private:
	UniformMap uniform_map;
}
