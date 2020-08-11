#include "Terrain.h"

Terrain::Terrain(float gridX, float gridZ) :
	x(gridX * SIZE), z(gridZ * SIZE), mesh(MESH_NAME_TERRAIN, VERTEX_COUNT * VERTEX_COUNT, (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1) * 2)
{
	int count = VERTEX_COUNT * VERTEX_COUNT;
	std::vector<vec3> vertices(count);
	std::vector<vec2> textures(count);
	std::vector<vec3> normals(count);
	std::vector<Face> faces;

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			vertices[vertexPointer] = vec3();
			vertices[vertexPointer].x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer].y = 0;
			vertices[vertexPointer].z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;

			textures[vertexPointer] = vec2();
			textures[vertexPointer].x = (float)j / ((float)VERTEX_COUNT - 1);
			textures[vertexPointer].y = (float)i / ((float)VERTEX_COUNT - 1);

			normals[vertexPointer] = vec3(0, 1, 0);

			vertexPointer++;
		}
	}

	for (int z = 0; z < VERTEX_COUNT - 1; z++)
	{
		for (int x = 0; x < VERTEX_COUNT - 1; x++)
		{
			int topLeft = z * VERTEX_COUNT + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * VERTEX_COUNT + x;
			int bottomRight = bottomLeft + 1;

			faces.push_back(Face());
			faces[faces.size() - 1].vertexIndices.setCoordinate(0, topLeft);
			faces[faces.size() - 1].vertexIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].vertexIndices.setCoordinate(2, topRight);
			faces[faces.size() - 1].textureIndices.setCoordinate(0, topLeft);
			faces[faces.size() - 1].textureIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].textureIndices.setCoordinate(2, topRight);
			faces[faces.size() - 1].normalIndices.setCoordinate(0, topLeft);
			faces[faces.size() - 1].normalIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].normalIndices.setCoordinate(2, topRight);

			faces.push_back(Face());
			faces[faces.size() - 1].vertexIndices.setCoordinate(0, topRight);
			faces[faces.size() - 1].vertexIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].vertexIndices.setCoordinate(2, bottomRight);
			faces[faces.size() - 1].textureIndices.setCoordinate(0, topRight);
			faces[faces.size() - 1].textureIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].textureIndices.setCoordinate(2, bottomRight);
			faces[faces.size() - 1].normalIndices.setCoordinate(0, topRight);
			faces[faces.size() - 1].normalIndices.setCoordinate(1, bottomLeft);
			faces[faces.size() - 1].normalIndices.setCoordinate(2, bottomRight);
		}
	}
	
	mesh.vertices = vertices;
	mesh.textures = textures;
	mesh.normals = normals;
	mesh.faces = faces;

	std::cout << "Processed input file (\"" <<
		"Terrain" <<
		"\": " <<
		faces.size() <<
		" faces, " <<
		vertices.size() <<
		" vertices)" <<
		std::endl;
}