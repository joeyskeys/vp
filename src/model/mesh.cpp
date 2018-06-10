#include "mesh.h"

#include <utility>

MeshManager* Mesh::mgr = MeshManager::getInstance();

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
    m->norms = b.m->norms;
    m->idx = b.m->idx;
    vert_cnt = b.vert_cnt;
    norm_cnt = b.norm_cnt;
    idx_cnt = b.idx_cnt;
}

Mesh& Mesh::operator=(const Mesh& b)
{
    m->verts = b.m->verts;
    m->norms = b.m->norms;
    m->idx = b.m->idx;
    vert_cnt = b.vert_cnt;
    norm_cnt = b.norm_cnt;
    idx_cnt = b.idx_cnt;

	return *this;
}

void Mesh::fillTestData()
{
    static float v[9] = {
        -0.5f, -0.5f, -1.0f,
         0.5f, -0.5f, -1.0f,
         0.0f,  0.5f, -1.0f
    };

    static float n[9] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    static int i[3] = {
        0, 1, 2
    };

    m->verts.fillData(9, v);
    m->norms.fillData(9, n);
    m->idx.fillData(3, i);
    vert_cnt = 3;
    norm_cnt = 3;
    idx_cnt = 1;
}

const float* Mesh::getVerts() const
{
    return m->verts.data;
}

const float* Mesh::getNorms() const
{
    return m->norms.data;
}

const int* Mesh::getIdx() const
{
    return m->idx.data;
}
