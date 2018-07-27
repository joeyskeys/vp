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
	// first loop, add all verts
	for (int i = 0; i < vt_cnt; i++)
	{
		float *v = m_verts + 3 * i;
		addVert(m_verts + 3 * i);
	}

	int tri_cnt = m->getTriCount();
	unsigned int *indices_base = m->getIdx();
	// second loop, create edges faces and loops
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
			DEdge *e = addEdge(v1, v2);

			new_loops[j] = addLoop(v1, v2, e, f);
			f->loops[j] = new_loops[j];
		}

		for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
		{
			new_loops[j]->next = new_loops[old_idx];
			DVert *v = vcache.getPtrOfIdx(indices[j]);
			if (!v->loop)
				v->loop = new_loops[j];
		}
	}

	int cnt = lcache.getCount();
	DLoop *end = lcache.data + cnt - 1;
	DLoop *tmp;
	// third loop, fill loop disk link info
	for (DLoop *iter = lcache.data; iter <= end; iter++)
	{
		iter->evalDiskNext();
		iter->evalDiskPrev();
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
	DLoop *l = v1->loop;
	while (l)
	{
		if (l->end == v2)
			return l->edge;
		l = l->disk_next;
	}
	l = v1->loop;
	while (l)
	{
		if (l->end == v2)
			return l->edge;
		l = l->disk_prev;
	}

	DEdge *e = ecache.useNext();
	e->updateLength(v1, v2);
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
	// create new vert
	DLoop *l = e->getAvailableLoop();
	glm::vec3 co = glm::make_vec3(l->start->co) - glm::make_vec3(l->end->co);
	co *= 0.5f;
	mesh->insertVert(glm::value_ptr(co));
	DVert *v = addVert(mesh->getVertAt(mesh->getVertCount() - 1));
	DEdge *new_e = addEdge(v, l->start);  // alloc for left new edge
	e->updateLength(v, l->end);

	DEdge *new_side_edges[2];
	new_side_edges[0] = new_e;
	new_side_edges[1] = e;
	DEdge *new_mid_edges[2];
	DFace *new_faces[2];
	DLoop *new_side_loops[2];
	new_side_loops[0] = nullptr;
	new_side_loops[1] = nullptr;
	DLoop *new_mid_loops[4];
	for (int i = 0; i < 4; i++)
		new_mid_loops[i] = nullptr;
	DLoop *old_loops[2];
	old_loops[0] = e->loops[0];
	old_loops[1] = e->loops[1];

	for (int i = 0; i < 2; i++)
	{
		DLoop *current_l = old_loops[i];
		if (current_l == nullptr)
			continue;

		/*			a
				   /|\
		          / | \
		         /  |  \
				/   |   \
		       b----v----c
		*/

		DVert *a = current_l->next->end;
		DVert *b = current_l->start;
		DVert *c = current_l->end;
		DLoop *la = current_l->next->next;
		DLoop *lb = current_l;
		DLoop *lc = current_l->next;
		int idx = mesh->getTriCount();
		unsigned int indices1[3] = {v->idx, a->idx, b->idx};
		mesh->insertTriangle(indices1);
		
		// new triangle for the left part
		// create elements
		new_mid_edges[i] = addEdge(v, a);
		new_faces[i] = addFace(mesh->getIdxAt(idx));
		new_side_loops[i] = addLoop(b, v, new_side_edges[i], new_faces[i]);
		new_mid_loops[i << 1] = addLoop(v, a, new_mid_edges[i], new_faces[i]);
		DLoop *current_mid_loop = new_mid_loops[i << 1];
		// fill in blank fields
		new_side_edges[i]->loops[0] = new_side_loops[i];
		new_side_edges[i]->loops[1] = old_loops[(i + 1) & 0b1];
		new_mid_edges[i]->loops[0] = current_mid_loop;
		new_faces[i]->loops[0] = new_side_loops[i];
		new_faces[i]->loops[1] = current_mid_loop;
		new_faces[i]->loops[2] = la;
		new_side_loops[i]->next = current_mid_loop;
		current_mid_loop->next = la;

		// modify old element values for right part
		// get ptrs and create neccessary elements;
		DFace *old_face = current_l->face;
		new_mid_loops[i << 1 + 1] = addLoop(a, v, new_mid_edges[i], new_faces[i]);
		current_mid_loop = new_mid_loops[i << 1 + 1];
		// update fields;
		new_mid_edges[i]->loops[1] = current_mid_loop;
		old_face->loops[old_face->getIdxOfLoop(la)] = current_mid_loop;
		current_l->start = v;
		lc->next = current_mid_loop;
		current_mid_loop->next = current_l;
	}

	for (int i = 0; i < 4; i++)
	{
		new_mid_loops[i]->evalDiskNext();
		new_mid_loops[i]->evalDiskPrev();
	}
	for (int i = 0; i < 2; i++)
	{
		new_side_loops[i]->evalDiskNext();
		new_side_loops[i]->evalDiskPrev();
		old_loops[i]->evalDiskNext();
		old_loops[i]->evalDiskPrev();
	}
}

void DynamicMesh::edgeCollapse(DEdge *e)
{
	DLoop *l = e->getAvailableLoop();
	glm::vec3 v = (glm::make_vec3(l->start->co) + glm::make_vec3(l->end->co)) / 2.f;
	l->start->setValue(&v);

	DLoop *old_loops[2];
	old_loops[0] = e->loops[0];
	old_loops[1] = e->loops[1];

	for (int i = 0; i < 2; i++)
	{
		DLoop *current_l = old_loops[i];
	}
}
