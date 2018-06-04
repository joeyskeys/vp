#pragma once

#include "cache.h"
#include "manager.h"

typedef struct MeshObj
{
	Cachef verts;
	Cachei idx;
} MeshObj;

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(const Mesh& b);
	Mesh& operator=(const Mesh& b);

	void read(Mesh *mptr);
	void write(Mesh *mptr);
	void fillTestData();

	const float*	getVerts() const;
	const int*		getIdx() const;
	inline const int 	getVertCount() const {return vert_cnt;}
	inline const int 	getIdxCount() const {return idx_cnt;}

private:
	MeshObj *m;
	int		vert_cnt;
	int		idx_cnt;

    static const MeshManager *mgr = MeshManager.getInstance();
};
