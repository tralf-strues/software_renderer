#include"ResLoader.h"

#include <SDL_image.h>
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

	cout << "Processed input file (\"" <<
		fileName <<
		"\": " <<
		mesh.faces.size() <<
		" faces, " <<
		mesh.vertices.size() <<
		" vertices)" <<
		endl;

	return mesh;
}

Texture loadTexture(string textureName, string fileNameWithExtension)
{
	SDL_Surface* surface = NULL;
	surface = SDL_LoadBMP(("res/" + fileNameWithExtension).c_str());

	int width = surface->w;
	int height = surface->h;
	vector<vec4> pixels(width * height);

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(surface)) {
		if (SDL_LockSurface(surface) < 0)
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
	}
	else
	{
		/*Uint32* surfacePixels = (Uint32*)surface->pixels;
		int surfacePitch = surface->pitch;
		Uint32* target_pixel;
		vec4 pixel;*/
		/*Uint32* surfacePixels = (Uint32*)surface->pixels;
		SDL_PixelFormat* surfacePixelFormat = surface->format;
		Uint8 r, g, b, a;*/
		Uint32* target_pixel;
		Uint8* surfacePixels = (Uint8*)surface->pixels;
		int surfacePitch = surface->pitch;
		SDL_PixelFormat* surfacePixelFormat = surface->format;
		Uint8 r, g, b, a;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				/*target_pixel = (Uint32*)(surfacePixels + y * surfacePitch + x * sizeof(*target_pixel));
				SDL_GetRGBA((Uint32)target_pixel, surfacePixelFormat, &r, &g, &b, &a);
				pixel = vec4((float)r, (float)g, (float)b, (float)a);
				pixels[y * width + x] = pixel;*/
				/*Uint32 pixel = surfacePixels[(y * width) + x];
				SDL_GetRGBA(pixel, surfacePixelFormat, &r, &g, &b, &a);
				pixels[y * width + x] = vec4((float)r, (float)g, (float)b, (float)a);*/

				target_pixel = (Uint32*)(surfacePixels +
										 y * surfacePitch +
										 x * surfacePixelFormat->BytesPerPixel);
				SDL_GetRGBA(*target_pixel, surfacePixelFormat, &r, &g, &b, &a);
				pixels[y * width + x] = vec4((float)r, (float)g, (float)b, (float)a);
			}
		}

		if (SDL_MUSTLOCK(surface)) {
			SDL_UnlockSurface(surface);
		}
	}

	return Texture(textureName, width, height, pixels);
}