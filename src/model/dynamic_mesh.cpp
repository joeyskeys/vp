#include "dynamic_mesh.h"

#include <utility>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

DynamicMesh::DynamicMesh(const Mesh *m):
	mesh(const_cast<Mesh*>(m))
{
	int vt_cnt = m->getVertCount();
	float *m_verts = m->getVerts();
	for (int i = 0; i < vt_cnt; i++)
	{
		float *v = m_verts + 3 * i;
		addVert(m_verts + 3 * i);
	}

	int tri_cnt = m->getTriCount();
	unsigned int *indices_base = m->getIdx();
	for (int i = 0; i < tri_cnt; i++)
	{
		unsigned int *indices = indices_base + 3 * i;
		DFace *f = addFace(indices);
		DLoop *new_loops[3];
		
		for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
		{
			DVert *v1 = vcache.getPtrOfIdx(indices[j]);
			DVert *v2 = vcache.getPtrOfIdx(old_idx);

			DLoop *l = v1->loop;
			DEdge *e = nullptr;
			do
			{
				if (l->end == v2)
				{
					e = l->edge;
					break;
				}
				l = l->disk_link;
			} while (l != v1->loop);
			if (!e)
				e = addEdge(v1, v2);

			new_loops[j] = addLoop(v1, v2, e, f);
			f->loops[j] = new_loops[j];
		}

		for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
		{
			new_loops[j]->next = new_loops[(j + 1) & 0b11];
			new_loops[j]->disk_link = new_loops[j]->edge->getAnotherLoop(new_loops[j]);
			DVert *v = vcache.getPtrOfIdx(indices[j]);
			if (!v->loop)
				v->loop = new_loops[j];
		}
	}
	

}

DynamicMesh::~DynamicMesh()
{

}

DynamicMesh::DynamicMesh(DynamicMesh&& b):
	mesh(b.mesh),
	vcache(std::move(b.vcache)),
	ecache(std::move(b.ecache)),
	fcache(std::move(b.fcache)),
	lcache(std::move(b.lcache))
{

}

DynamicMesh& DynamicMesh::operator=(DynamicMesh&& b)
{
	mesh = b.mesh;
	vcache = std::move(b.vcache);
	ecache = std::move(b.ecache);
	fcache = std::move(b.fcache);
	lcache = std::move(b.lcache);

    return *this;
}

DVert* DynamicMesh::addVert(float *c)
{
	DVert *v = vcache.useNext();
	v->co = c;
	v->idx = vcache.getIdxOfPtr(v);
	return v;
}

DEdge* DynamicMesh::addEdge(DVert *v1, DVert *v2)
{
	DEdge *e = ecache.useNext();
	e->length = glm::length(glm::make_vec3(v1->co) - glm::make_vec3(v2->co));
	return e;
}

DFace* DynamicMesh::addFace(unsigned int *i)
{
	DFace *f = fcache.useNext();
	f->idx = i;
	return f;
}

DLoop* DynamicMesh::addLoop(DVert *st, DVert *ed, DEdge *e, DFace *f)
{
	DLoop *l = lcache.useNext();
	l->start = st;
	l->end = ed;
	l->edge = e;
	l->face = f;
	return l;
}

void DynamicMesh::addTriangle(unsigned int *idx)
{

}

void DynamicMesh::edgeSplit(DEdge *e)
{
	DLoop *l = e->getAvailableLoop();
	glm::vec3 co = glm::make_vec3(l->start->co) - glm::make_vec3(l->end->co);
	co *= 0.5f;
	mesh->insertVert(glm::value_ptr(co));
	DVert *v = addVert(mesh->getVertAt(mesh->getVertCount() - 1));

	for (int i = 0; i < 2; i++)
	{
		DLoop *current_l = e->loops[i];
		if (current_l == nullptr)
			continue;

		unsigned int indices1[3] = {l->end->idx, l->next->end->idx, v->idx};
		mesh->insertTriangle(indices1);
		addTriangle(indices1);

		e->length = glm::length(co - glm::make_vec3(current_l->start->co));
		current_l->end = v;
	}
}

void DynamicMesh::edgeCollapse(DEdge *e)
{

}
