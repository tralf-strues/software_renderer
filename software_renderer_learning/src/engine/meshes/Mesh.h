#pragma once

#include "../math/engineMath.h"
#include "Face.h"
#include <vector>

struct Mesh {
	Mesh(std::string name, int verticesCount, int facesCount);
	~Mesh();

	std::string name;
	std::vector<vec3> vertices;
	std::vector<vec2> textures;
	std::vector<vec3> normals;
	std::vector<Face> faces;
	vec3 position;
	vec3 rotation;

	void setScale(float newScale);

private:
	float scale;
};