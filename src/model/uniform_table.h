#include "utils.h"

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

using std::pair<int, void*> = UniformPair;
using std::map<std::string, Uniform> = UniformMap;

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

	void setValue(void* src);

private:
	size_t size;
	AutoBuffer buf;
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

	bool loadDescription(std::string& filepath)

private:
	UniformMap uniform_map;
}