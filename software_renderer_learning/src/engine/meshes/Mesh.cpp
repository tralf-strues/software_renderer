#include "Mesh.h"

Mesh::Mesh(std::string name, int verticesCount, int facesCount) :
	name(name), 
	vertices(std::vector<vec3>(verticesCount)),
	faces(std::vector<Face>(facesCount))
{
}

Mesh::~Mesh()
{
}