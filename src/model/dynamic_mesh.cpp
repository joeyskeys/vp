#include "dynamic_mesh.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

DynamicMesh::DynamicMesh(const Mesh *m):
	mesh(const_cast<Mesh*>(m))
{

}

DynamicMesh::~DynamicMesh()
{

}

DynamicMesh::DynamicMesh(const DynamicMesh& b)
{

}

DynamicMesh& DynamicMesh::operator=(const DynamicMesh& b)
{

}

DynamicMesh::DynamicMesh(DynamicMesh&& b)
{

}

DynamicMesh& DynamicMesh::operator=(DynamicMesh&& b)
{

}

DVert* DynamicMesh::addVert(float *c)
{
	DVert *v = vcache.useNext();
	v->co = &c;
	return v;
}

DEdge* DynamicMesh::addEdge(DVert *v1, DVert *v2)
{
	DEdge *e = ecache.useNext();
	e->length = glm::length(glm::vec3(*(v1->x), *(v1->y), *(v1->z)) - glm::vec3(*(v2->x), *(v2->y), *(v2->z)));
	return e;
}

DFace* DynamicMesh::addFace(unsigned int *i)
{
	DFace *f = fcache.useNext();
	f->idx = &i;
	return f;
}

DLoop* DynamicMesh::addLoop(DVert *st, DVert *ed, DEdge *e, DFace *f)
{
	DLoop *l = lcache.useNext();
	l->start = st;
	l->end = ed;
	l->edge = e;
	l->face = f;

}