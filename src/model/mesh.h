#pragma once

#include "cache.h"
#include "manager.h"

typedef struct MeshObj
{
	Cachef verts;
	Cachef norms;
	Cacheu idx;
} MeshObj;

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(const Mesh& b);
	Mesh& operator=(const Mesh& b);

	void fillTriangle();
	void fillCube();

	inline float*		getVerts() const { return m->verts.data; }
	inline float*		getVertAt(int i) const { return m->verts.data + 3 * i; }
	inline float*		getNorms() const { return m->norms.data; }
	inline float*		getNormAt(int i) const { return m->norms.data + 3 * i; }
	inline unsigned int*	getIdx() const { return m->idx.data; }
	inline unsigned int*	getIdxAt(int i) const { return m->idx.data + 3 * i; }
	inline const int 	getVertCount() const { return vert_cnt; }
	inline const int 	getNormCount() const { return norm_cnt; }
	inline const int	getTriCount() const { return tri_cnt; }
	inline const size_t	getVertSize() const { return vert_cnt * sizeof(float) * 3; }
	inline const size_t getNormSize() const { return norm_cnt * sizeof(float) * 3; }
	inline const size_t getIdxSize() const	{ return idx_cnt * sizeof(int) * 3; }

	void				insertVert(float *v);
	void				insertVerts(float *v, int cnt);
	void				insertNorm(float *n);
	void				insertNorms(float *n, int cnt);
	void				insertTriangle(unsigned int *i);
	void				insertTriangles(unsigned int *i, int cnt);
	inline void			deleteVert(float *v) { m->verts.deletePtrWithCnt(v, 3); }
	inline void			deleteVerts(float *v, int cnt) { m->verts.deletePtrWithCnt(v, 3 * cnt); }
	inline void			deleteNorm(float *n) { m->norms.deletePtrWithCnt(n, 3); }
	inline void			deleteNorms(float *n, int cnt) { m->norms.deletePtrWithCnt(n, 3 * cnt); }
	inline void			deleteTriangle(unsigned int *i) { m->idx.deletePtrWithCnt(i, 3); }
	inline void			deleteTriangles(unsigned int *i, int cnt) { m->idx.deletePtrWithCnt(i, 3 * cnt); }

private:
	MeshObj *m;
	int		vert_cnt;
	int 	norm_cnt;
	int		idx_cnt;
	int		tri_cnt;

    static MeshManager *mgr;
};
