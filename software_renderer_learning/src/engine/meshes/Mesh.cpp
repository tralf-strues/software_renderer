#include "Mesh.h"

Mesh::Mesh(std::string name, int verticesCount, int facesCount) :
	name(name), 
	vertices(std::vector<vec3>(verticesCount)),
	faces(std::vector<Face>(facesCount)),
	scale(1)
{
}

Mesh::~Mesh()
{
}

void Mesh::setScale(float newScale)
{
	if (newScale <= 0)
		return;

	float multiplier = newScale / scale;
	for (int i = 0; i < vertices.size(); i++)
		vertices[i] *= multiplier;

	scale = newScale;
}