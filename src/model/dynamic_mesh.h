#include "mesh.h"
#include "cache.h"

typedef struct DVert DVert;
typedef struct DEdge DEdge;
typedef struct DFace DFace;
typedef struct DLoop DLoop;

typedef Cache<DVert> DVCache;
typedef Cache<DEdge> DECache;
typedef Cache<DFace> DFCache;
typedef Cache<DLoop> DLCache;

struct DVert
{
	float **co;
	DEdge *edges[3];
};

struct DEdge
{
	float		 length;
	DLoop		 *loop1, *loop2;
};

struct DFace
{
	unsigned int **idx;
	DLoop		 *loops[3];
};

struct DLoop
{
	DVert		*start;
	DVert		*end;
	DEdge		*edge;
	DFace		*face;
	DLoop		*next;
};

class DynamicMesh
{
public:
	explicit DynamicMesh(const Mesh *m);
	~DynamicMesh();
	DynamicMesh(const DynamicMesh& b);
	DynamicMesh& operator=(const DynamicMesh& b);
	DynamicMesh(DynamicMesh&& b);
	DynamicMesh& operator=(DynamicMesh&& b);

private:
	DVert*	addVert(float *c);
	DEdge*	addEdge(DVert *v1, DVert *v2);
	DFace*	addFace(unsigned int *id);
	DLoop*	addLoop(DVert *st, DVert *ed, DEdge *e, DFace *f);
	void	addTriangle(DFace *f, DEdge *es, DLoop *ls);

public:
	void	edgeSplit(DEdge *e);
	void	edgeCollapse(DEdge *e);

private:
	Mesh	*mesh;
	DVCache vcache;
	DECache ecache;
	DFCache fcache;
	DLCache lcache;
};