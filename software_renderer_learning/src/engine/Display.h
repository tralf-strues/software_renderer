#pragma once
#include <SDL.h>
#include <vector>

#include "meshes/Mesh.h"
#include "entities/Camera.h"

class Display
{
private:
	SDL_Surface *surface;
	int width; 
	int height;
	std::vector<float> depthBuffer;

	Uint8 *surfacePixels;
	int surfacePitch;
	SDL_PixelFormat *surfacePixelFormat;

public:
	Display(SDL_Window* window, int width, int height);
	Display();
	~Display();

	int getWidth();
	int getHeight();
	SDL_Surface* getSurface();

	void render(Camera& camera, std::vector<Mesh>& meshes,
				   bool isPerspective, bool isWireframe);
private:
	void clear(vec4 color);
	vec3 project(vec3 coordinates, mat4 transformationMatrix);
	void drawPoint(vec2 coordinates, vec4 color);
	void drawLine(vec2 p0, vec2 p1, vec4 color);
	void drawTriangle(vec3 p1, vec3 p2, vec3 p3, vec4 color);
	void processScanLine(int y, vec3 l1, vec3 l2, vec3 r1, vec3 r2, vec4 color);
	
};