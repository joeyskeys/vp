#pragma once

#include "mesh.h"
#include "cache.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <deque>
#include <set>

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

	inline void setValue(float *v) { for (int i = 0; i < 3; i++) co[i] = v[i]; }
	inline void setValue(glm::vec3 *v) { co[0] = v->x; co[1] = v->y; co[2] = v->z; }

	glm::vec3	operator-(DVert& b) { return glm::make_vec3(co) - glm::make_vec3(b.co); }
};

struct DEdge
{
	float		 length;
	DLoop		 *loops[2];

	inline void	  updateLength(DVert *v1, DVert *v2) { length = glm::length(glm::make_vec3(v1->co) - glm::make_vec3(v2->co)); }
	inline DLoop* getAvailableLoop() { return loops[0] ? loops[0] : loops[1]; }
	inline DLoop* getAnotherLoop(DLoop *l) { return loops[0] == l ? loops[1] : loops[0]; }
	inline int	  getIdxOfLoop(DLoop *l) { return loops[0] == l ? 0 : 1; }
    inline int    getAnotherIdxOfLoop(DLoop *l) { return loops[0] == l ? 1 : 0; }
};

struct DFace
{
	unsigned int *idx;
	DLoop		 *loops[3];

	inline int    getIdxOfLoop(DLoop *l) { return loops[0] == l ? 0 : (loops[1] == l ? 1 : 2); }
	glm::vec3	  getNormal();
};

struct DLoop
{
	DVert		*start;
	DVert		*end;
	DEdge		*edge;
	DFace		*face;
	DLoop		*next;
	DLoop		*disk_next;
	DLoop		*disk_prev;

	inline void	  evalDiskNext() { DLoop *tmp = next->next; disk_next = tmp->edge->getAnotherLoop(tmp); }
	inline void   evalDiskPrev() { DLoop *tmp = edge->getAnotherLoop(this); disk_prev = tmp ? tmp->next : nullptr; }
	inline glm::vec3 getVector() { return *end - *start; }
};

using VerticesSet = std::map<DVert*, std::pair<glm::vec3, float>>;
using FaceSet = std::set<DFace*>;
using LoopQueue = std::deque<DLoop*>;

class DynamicMesh
{
public:
	DynamicMesh();
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
	inline void	  deleteVert(DVert *v) { vcache.deletePtr(v); /* mesh->deleteVert(v->co);*/} // delete vertex directly will cause indices being messed up
	inline void   deleteEdge(DEdge *e) { ecache.deletePtr(e); }
	inline void   deleteFace(DFace *f) { fcache.deletePtr(f); mesh->deleteTriangle(f->idx); }
	inline void	  deleteLoop(DLoop *l) { lcache.deletePtr(l); }

public:
	void	addTriangle(unsigned int *idx);
	void	edgeSplit(DEdge *e);
	void	edgeCollapse(DEdge *e);
	VerticesSet getAffectedVertices(glm::vec3& center, int face_idx, float radius);

private:
	Mesh	*mesh;
	DVCache vcache;
	DECache ecache;
	DFCache fcache;
	DLCache lcache;
};
