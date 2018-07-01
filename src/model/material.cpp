#include "material.h"

#include <fstream>
#include <utility>

#include <QOpenGLShader>
#include <QDebug>

using namespace std;

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
	ShaderProgram tmp;
	for (auto path : search_paths)
	{
		auto ret = tmp.load(path, name);
		if (ret)
		{
			shader_map.insert(move(make_pair(name, move(tmp))));
			return true;
		}
	}

	return false;
}

ShaderProgram* ShaderManager::getShader(const string& name)
{
    auto it = shader_map.find(name);
    if (it != shader_map.end())
        return &(it->second);
    else
        return nullptr;
}
