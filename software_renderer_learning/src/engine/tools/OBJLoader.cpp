#include"OBJLoader.h"

#include <iostream>
#include <fstream>

using namespace std;

vector<string> split(string s, string separator)
{
	vector<string> splitted;
	int last = 0;
	int next = 0;
	while ((next = s.find(separator, last)) != string::npos)
	{
		splitted.push_back(s.substr(last, next - last));
		last = next + 1;
	} 

	splitted.push_back(s.substr(last, s.size() - last));

	return splitted;
}

Mesh loadOBJ(string meshName, string fileName)
{
	ifstream in("res/" + fileName + ".obj");

	if (!in) 
	{
		cout << "Cannot open input file.\n";
		in.close();
		return Mesh("FAILED", 0, 0);
	}

	cout << "Opened input file.\n";
	vector<vec3> vertices;
	vector<vec2> textures;
	vector<vec3> normals;
	vector<Face> faces;

	string line;
	vector<string> splittedLine;
	vector<string> splittedFace;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			splittedLine = split(line, " ");
			vertices.push_back(vec3(stof(splittedLine.at(1)),
									stof(splittedLine.at(2)),
									stof(splittedLine.at(3))));
		}
		else if (line.substr(0, 3) == "vt ")
		{
			splittedLine = split(line, " ");
			textures.push_back(vec2(stof(splittedLine.at(1)),
									stof(splittedLine.at(2))));
		}
		else if (line.substr(0, 3) == "vn ")
		{
			splittedLine = split(line, " ");
			normals.push_back(vec3(stof(splittedLine.at(1)),
								   stof(splittedLine.at(2)),
								   stof(splittedLine.at(3))));
		}
		else if (line.substr(0, 2) == "f ")
		{
			splittedLine = split(line, " ");

			faces.push_back(Face());
			for (int i = 1; i < 4; i++)
			{
				splittedFace = split(splittedLine.at(i), "/");
				faces[faces.size() - 1].vertexIndices.setCoordinate(i - 1, stof(splittedFace.at(0)) - 1);
				faces[faces.size() - 1].textureIndices.setCoordinate(i - 1, stof(splittedFace.at(1)) - 1);
				faces[faces.size() - 1].normalIndices.setCoordinate(i - 1, stof(splittedFace.at(2)) - 1);
			}
		}
	}

	in.close();

	Mesh mesh(meshName, vertices.size(), faces.size());
	mesh.vertices = vertices;
	mesh.textures = textures;
	mesh.normals = normals;
	mesh.faces = faces;

	return mesh;
}