#include "dynamic_mesh.h"

#include <utility>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 DFace::getNormal()
{
	glm::vec3 a = loops[0]->getVector();
	glm::vec3 b = -(loops[0]->next->next->getVector());
	return glm::normalize(glm::cross(a, b));
}

DynamicMesh::DynamicMesh():
	mesh(nullptr)
{

}

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
        // add face
		unsigned int *indices = indices_base + 3 * i;
		DFace *f = addFace(indices);
		DLoop *new_loops[3];
		
        // create loops
		for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
		{
			DVert *v1 = vcache.getPtrOfIdx(indices[j]);
			DVert *v2 = vcache.getPtrOfIdx(old_idx);

			new_loops[j] = addLoop(v1, v2, nullptr, f);
			f->loops[j] = new_loops[j];
		}

        // fill in loop->next
		for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
		{
			new_loops[old_idx]->next = new_loops[j];
			DVert *v = vcache.getPtrOfIdx(indices[old_idx]);
			v->loops.insert(new_loops[j]);
		}
	}

    // third loop, create edges
    for (int i = 0; i < tri_cnt; i++)
    {
        unsigned int *indices = indices_base + 3 * i;
        for (int old_idx = 2, j = 0; j < 3; old_idx = j, j++)
        {
            DVert *v1 = vcache.getPtrOfIdx(indices[j]);
            DVert *v2 = vcache.getPtrOfIdx(old_idx);
            addEdge(v1, v2);
        }
    }

	int cnt = lcache.getCount();
	DLoop *end = lcache.data + cnt - 1;
	DLoop *tmp;
	// fourth loop, fill loop disk link info
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
	std::cout << "in add vert" << std::endl;
	DVert *v = vcache.useNext();
	v->co = c;
	v->idx = vcache.getIdxOfPtr(v);
	return v;
}

DEdge* DynamicMesh::addEdge(DVert *v1, DVert *v2)
{
    for (auto it = v1->loops.begin(); it != v1->loops.end(); ++it)
    {
        if ((*it)->end == v2)
        {
            if ((*it)->edge != nullptr)
                return (*it)->edge;
            else
                break;
        }
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
    st->loops.insert(l);
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
    // arrange the loop order with respect to the first found loop
	old_loops[0] = l;
	old_loops[1] = e->getAnotherLoop(l);

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
		new_mid_loops[(i << 1) + 1] = addLoop(a, v, new_mid_edges[i], new_faces[i]);
		current_mid_loop = new_mid_loops[(i << 1) + 1];
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
    // adjust the first found loop's start vertex into new
	DLoop *l = e->getAvailableLoop();
	glm::vec3 co = (glm::make_vec3(l->start->co) + glm::make_vec3(l->end->co)) / 2.f;
    DVert *v = l->start;
    DVert *remove_v = l->end;
    v->setValue(&co);

	DLoop *old_loops[2];
	old_loops[0] = l;
	old_loops[1] = e->getAnotherLoop(l);

	for (int i = 0; i < 2; i++)
	{
		DLoop *current_l = old_loops[i];
        if (!current_l)
            continue;
        
        DLoop *remove_l_left, *remove_l_right;
        DEdge *remove_e, *preserve_e;
        if (current_l->start == v)
        {
            remove_e = current_l->next->edge;
            preserve_e = current_l->next->next->edge;
            remove_l_left = current_l->next->next;
            remove_l_right = current_l->next;
        }
        else
        {
            remove_e = current_l->next->next->edge;
            preserve_e = current_l->next->edge;
            remove_l_left = current_l->next;
            remove_l_right = current_l->next->next;
        }
        
        preserve_e->loops[preserve_e->getAnotherIdxOfLoop(remove_l_left)] = remove_e->getAnotherLoop(remove_l_right);

        // delete loops, current_l will be invalid since then
        DFace *f = current_l->face;
        for (int i = 0; i < 3; i++)
            deleteLoop(f->loops[i]);
        deleteFace(f);
        deleteEdge(remove_e);
	}
    
    // delete the left elements
    deleteVert(remove_v);
    deleteEdge(e);
}

static float projectedDistance(const glm::vec3& normal, const glm::vec3 center, const glm::vec3 pos)
{
	glm::vec3 N = glm::normalize(normal);
	glm::vec3 V = pos - center;
	return glm::length(V - glm::dot(V, N) * N);
}

VerticesSet DynamicMesh::getAffectedVertices(glm::vec3& center, int face_idx, float radius)
{
	DFace current_face = fcache[face_idx];
	VerticesSet affected_verts;
	LoopQueue 	loop_queue;
	FaceSet 	face_set;

	// apply bfs search to find all affected vertices
	face_set.insert(&current_face);
	for (int i = 0; i < 3; i++)
	{
		DLoop *l = current_face.loops[i];
		loop_queue.push_back(l);
		DVert *v = l->start;
		affected_verts.emplace(make_pair(v, make_pair(glm::make_vec3(v->co), 1.f)));
	}

	while (loop_queue.size() > 0)
	{
		DLoop *current_loop = loop_queue.front();
		DLoop *end = current_loop->next->next;
		end = end->edge->getAnotherLoop(end);
		DLoop *l = current_loop;
		loop_queue.pop_front();
		DVert* vert_ptr = nullptr;

		while (l->disk_prev != end && l->disk_prev != nullptr)
		{
			DFace *f = l->disk_prev->face;
			FaceSet::iterator it = face_set.find(f);
			if (it != face_set.end())
				continue;
			face_set.insert(f);

			DVert *v = l->disk_prev->end;
			glm::vec3 vt_co = glm::make_vec3(v->co);
			if (projectedDistance(f->getNormal(), center, vt_co) < radius)
			{
				affected_verts.emplace(make_pair(v, make_pair(vt_co, 1.f)));
				DLoop *new_loop = l->disk_prev->next;
				loop_queue.push_back(new_loop->edge->getAnotherLoop(new_loop));
			}
		}
	}

    return affected_verts;
}
