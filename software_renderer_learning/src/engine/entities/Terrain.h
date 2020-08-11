#pragma once

#include <vector>
#include "../math/vec3.h"
#include "../meshes/Mesh.h"
#include "../meshes/Face.h"

#define SIZE 128
#define VERTEX_COUNT 64
#define MESH_NAME_TERRAIN "Terrain"

class Terrain
{
private:
	float x;
	float z;

public:
	Terrain(float gridX, float gridZ);
	Mesh mesh;

};