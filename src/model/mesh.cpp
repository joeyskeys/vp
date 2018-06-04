#include <utility>

#include "mesh.h"

Mesh::Mesh()
{
    void *t = mgr->getNext();
    m = new (t) MeshObj;
}

Mesh::~Mesh()
{
    m->~MeshObj();
}

Mesh::Mesh(const Mesh& b)
{
    m->verts = b.m->verts;
    m->idx = b.m->idx;
    vert_cnt = b.vert_cnt;
    idx_cnt = b.idx_cnt;
}

Mesh& Mesh::operator=(const Mesh& b)
{
    m->verts = b.m->verts;
    m->idx = b.m->idx;
    vert_cnt = b.vert_cnt;
    idx_cnt = b.idx_cnt;

	return *this;
}

void Mesh::read(MeshObj *mptr)
{
    std::swap(m->verts, mptr->verts);
    std::swap(m->idx, mptr->idx);
    vert_cnt = mptr->verts.size / sizeof(float);
    idx_cnt = mptr->idx.size / sizeof(int);
}

void Mesh::write(MeshObj *mptr)
{
    std::swap(m->verts, mptr->verts);
    std::swap(m->idx, mptr->idx);
}

void Mesh::fillTestData()
{
    static float v[9] = {
        -0.5f, -0.5f, -1.0f,
         0.5f, -0.5f, -1.0f,
         0.0f,  0.5f, -1.0f
    };

    static int i[3] = {
        0, 1, 2
    };

    m->verts.fillData(9, v);
    m->idx.fillData(3, i);
    vert_cnt = 3;
    idx_cnt = 1;
}

const float* Mesh::getVerts() const
{
    return m->verts.data;
}

const int* Mesh::getIdx() const
{
    return m->idx.data;
}
