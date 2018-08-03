#include "obj_loader.h"

ObjLoader::ObjLoader()
{

}

ObjLoader::~ObjLoader()
{

}

ObjLoader::ObjLoader(const ObjLoader& rhs):
	verts(rhs.verts),
	norms(rhs.norms),
	indices(rhs.indices),
	objects(rhs.objects)
{

}

ObjLoader& ObjLoader::operator=(const ObjLoader& rhs)
{
	verts = rhs.verts;
	norms = rhs.norms;
	indices = rhs.indices;
	objects = rhs.objects;

	return *this;
}

void ObjLoader::load(std::string &path)
{

}