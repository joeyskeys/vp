#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utility>

MeshManager* Mesh::mgr = MeshManager::getInstance();

Mesh::Mesh():
    vert_cnt(0),
    norm_cnt(0),
    idx_cnt(0),
    tri_cnt(0)
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
	tri_cnt = b.tri_cnt;
}

Mesh& Mesh::operator=(const Mesh& b)
{
    m->verts = b.m->verts;
    m->norms = b.m->norms;
    m->idx = b.m->idx;
    vert_cnt = b.vert_cnt;
    norm_cnt = b.norm_cnt;
	tri_cnt = b.tri_cnt;

	return *this;
}

void Mesh::fillTriangle()
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

    static unsigned int i[3] = {
        0, 1, 2
    };

    m->verts.fillData(9, v);
    m->norms.fillData(9, n);
    m->idx.fillData(3, i);
    vert_cnt = 3;
    norm_cnt = 3;
    idx_cnt = 3;
	tri_cnt = 1;
}

void Mesh::fillCube()
{
	static float v[24] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f
	};

	static float n[24] = {
        -0.57735f, -0.57735f, -0.57735f,
         0.57735f, -0.57735f, -0.57735f,
         0.57735f, -0.57735f,  0.57735f,
        -0.57735f, -0.57735f,  0.57735f,
        -0.57735f,  0.57735f, -0.57735f,
         0.57735f,  0.57735f, -0.57735f,
         0.57735f,  0.57735f,  0.57735f,
        -0.57735f,  0.57735f,  0.57735f
	};

    static unsigned int i[36] = {
        // back
        0, 4, 5,
        0, 5, 1,
        // front
        3, 6, 7,
        3, 2, 6,
        // up
        4, 7, 5,
        5, 7, 6,
        // bottom
        0, 1, 3,
        3, 1, 2,
        // left
        0, 7, 4,
        0, 3, 7,
        // right
        6, 2, 1,
        6, 1, 5
    };
    
    m->verts.fillData(24, v);
    m->norms.fillData(24, n);
    m->idx.fillData(36, i);
    vert_cnt = 8;
    norm_cnt = 8;
    idx_cnt = 36;
	tri_cnt = 12;
}

void Mesh::updateExpandedMesh()
{
    expanded_verts.reserve(idx_cnt);
    expanded_norms.reserve(tri_cnt / 3);
    expanded_verts.clear();
    expanded_norms.clear();
    for (int i = 0; i < tri_cnt; i++)
    {
        glm::vec3 v1 = glm::make_vec3(m->verts.getPtrOfIdx(m->idx[tri_cnt * 3] * 3));
        glm::vec3 v2 = glm::make_vec3(m->verts.getPtrOfIdx(m->idx[tri_cnt * 3 + 1] * 3));
        glm::vec3 norm = glm::normalize(glm::cross(v1, v2));

        expanded_verts.appendData(glm::value_ptr(v1), 3);
        expanded_verts.appendData(glm::value_ptr(v2), 3);
        expanded_verts.appendData(m->verts.getPtrOfIdx(m->idx[tri_cnt * 3 + 2] * 3), 3);
        expanded_norms.appendData(glm::value_ptr(norm), 3);
    }
}

void Mesh::insertVert(float *v)
{
	float *buf = m->verts.useNext();
	buf[0] = v[0];
	buf[1] = v[1];
	buf[2] = v[2];
	vert_cnt++;
}

void Mesh::insertVerts(float *v, int cnt)
{
	m->verts.appendData(v, 3 * cnt);
	vert_cnt += cnt;
}

void Mesh::copyVerts(float *v, int cnt)
{
    m->verts.copyData(v, 3 * cnt);
    vert_cnt = cnt;
}

void Mesh::insertNorm(float *n)
{
	float *buf = m->norms.useNext();
	buf[0] = n[0];
	buf[1] = n[1];
	buf[2] = n[2];
	norm_cnt++;
}

void Mesh::insertNorms(float *n, int cnt)
{
	m->norms.appendData(n, 3 * cnt);
	norm_cnt += cnt;
}

void Mesh::copyNorms(float *n, int cnt)
{
    m->norms.copyData(n, 3 * cnt);
    norm_cnt = cnt;
}

void Mesh::insertTriangle(unsigned int *i)
{
	unsigned int *buf = m->idx.useNext();
	buf[0] = i[0];
	buf[1] = i[1];
	buf[2] = i[2];
	idx_cnt++;
}

void Mesh::insertTriangles(unsigned int *i, int cnt)
{
    int tmp = 3 * cnt;
	m->idx.appendData(i, tmp);
	idx_cnt += tmp;
    tri_cnt += cnt;
}

void Mesh::copyTriangles(unsigned int *i, int cnt)
{
    int tmp = 3 * cnt;
    m->idx.copyData(i, tmp);
    idx_cnt = tmp;
    tri_cnt = cnt;
}
