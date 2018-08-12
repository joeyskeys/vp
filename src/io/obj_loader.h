/*
 * A basic obj loader for now;
 * Used as a utility, only loads the mesh, omit the obj
 * format material.
 */

#include <model/mesh.h>

#include <vector>

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();
	ObjLoader(const ObjLoader& rhs);
	ObjLoader& operator=(const ObjLoader& rhs);

	void load(std::string &&path);
	void reset();
	void fillMesh(Mesh *mesh);
    void loadIntoMesh(Mesh *mesh);

    void printInfo();

private:
	std::vector<float> verts;
	std::vector<float> norms;
	std::vector<unsigned int> indices;
    std::vector<unsigned int> norm_indices;
	std::vector<unsigned int> objects;
};
