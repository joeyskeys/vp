#pragma once

#include "cache.h"
#include "manager.h"

typedef struct MeshObj
{
	Cachef verts;
	Cachef norms;
	Cachei idx;
} MeshObj;

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(const Mesh& b);
	Mesh& operator=(const Mesh& b);

	void fillTestData();

	const float*	getVerts() const;
	const float*	getNorms() const;
	const int*		getIdx() const;
	inline const int 	getVertCount() const { return vert_cnt; }
	inline const int 	getNormCount() const { return norm_cnt; }
	inline const int 	getIdxCount() const { return idx_cnt; }
	inline const size_t	getVertSize() const { return vert_cnt * sizeof(float) * 3; }
	inline const size_t getNormSize() const { return norm_cnt * sizeof(float) * 3; }
	inline const size_t getIdxSize() const	{ return idx_cnt * sizeof(int) * 3; }

private:
	MeshObj *m;
	int		vert_cnt;
	int 	norm_cnt;
	int		idx_cnt;

    static MeshManager *mgr;
};