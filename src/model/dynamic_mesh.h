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
	unsigned int idx;
	float *co;
	DLoop *loop;
};

struct DEdge
{
	float		 length;
	DLoop		 *loops[2];

	inline DLoop* getAvailableLoop() { return loops[0] ? loops[0] : loops[1]; }
	inline DLoop* getAnotherLoop(DLoop *l) { return loops[0] == l ? loops[1] : loops[0]; }
};

struct DFace
{
	unsigned int *idx;
	DLoop		 *loops[3];
};

struct DLoop
{
	DVert		*start;
	DVert		*end;
	DEdge		*edge;
	DFace		*face;
	DLoop		*next;
	DLoop		*disk_link;
};

class DynamicMesh
{
public:
	explicit DynamicMesh(const Mesh *m);
	~DynamicMesh();
	DynamicMesh(const DynamicMesh& b) = delete;
	DynamicMesh& operator=(const DynamicMesh& b) = delete;
	DynamicMesh(DynamicMesh&& b);
	DynamicMesh& operator=(DynamicMesh&& b);

private:
	DVert*	addVert(float *c);
	DEdge*	addEdge(DVert *v1, DVert *v2);
	DFace*	addFace(unsigned int *id);
	DLoop*	addLoop(DVert *st, DVert *ed, DEdge *e, DFace *f);

public:
	void	addTriangle(unsigned int *idx);
	void	edgeSplit(DEdge *e);
	void	edgeCollapse(DEdge *e);

private:
	Mesh	*mesh;
	DVCache vcache;
	DECache ecache;
	DFCache fcache;
	DLCache lcache;
};