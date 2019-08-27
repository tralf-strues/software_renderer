#include "Display.h"
#include <iostream>

Display::Display(SDL_Window* window, int width, int height) :
	width(width),
	height(height),
	depthBuffer(std::vector<float>(width * height))
{
	this->surface = SDL_GetWindowSurface(window);

	surfacePixels = (Uint8 *)surface->pixels;
	surfacePitch = surface->pitch;
	surfacePixelFormat = surface->format;
}

Display::Display() :
	width(100),
	height(100),
	depthBuffer(std::vector<float>(100 * 100))
{
	this->surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	surfacePixels = (Uint8 *)surface->pixels;
	surfacePitch = surface->pitch;
	surfacePixelFormat = surface->format;
}

Display::~Display()
{
}

int Display::getWidth()
{
	return width;
}

int Display::getHeight()
{
	return height;
}

SDL_Surface* Display::getSurface()
{
	return surface;
}

void Display::render(Camera camera, std::vector<Mesh> meshes,
					 bool isPerspective, bool isWireframe)
{
	clear(vec4(0, 0, 0, 1));

	//std::cout << "R";

	mat4 viewMatrix = mat4::lootAt(camera.position, camera.target);
	mat4 projectionMatrix;
	if (isPerspective)
		projectionMatrix = mat4::perspectiveFOV(0.78f, width / height, 5, 100);
	else
		projectionMatrix = mat4::orthographic(-5, 5, -5, 5, -5, 5);

	mat4 worldMatrix;
	mat4 transformMatrix;
	vec3 v0, v1, v2; // vertices
	vec3 p0, p1, p2; // pixels
	int faceIndex;
	for (Mesh mesh : meshes)
	{
		worldMatrix = mat4::translation(mesh.position)
			* mat4::rotationPitchYawRoll(mesh.rotation);
		transformMatrix = projectionMatrix * (viewMatrix * worldMatrix);

		faceIndex = 0;
		for (Face face : mesh.faces)
		{
			v0 = mesh.vertices[face.vertexIndices.x];
			v1 = mesh.vertices[face.vertexIndices.y];
			v2 = mesh.vertices[face.vertexIndices.z];

			p0 = project(v0, transformMatrix);
			p1 = project(v1, transformMatrix);
			p2 = project(v2, transformMatrix);

			p0.x = (int)(p0.x * (width / 2) + (width / 2));
			p0.y = (int)(-p0.y * (height / 2) + (height / 2));
			p1.x = (int)(p1.x * (width / 2) + (width / 2));
			p1.y = (int)(-p1.y * (height / 2) + (height / 2));
			p2.x = (int)(p2.x * (width / 2) + (width / 2));
			p2.y = (int)(-p2.y * (height / 2) + (height / 2));

			if (isWireframe)
			{
				drawLine(vec2(p0.x, p0.y), vec2(p1.x, p1.y), vec4(255, 255, 0, 255));
				drawLine(vec2(p0.x, p0.y), vec2(p2.x, p2.y), vec4(255, 255, 0, 255));
				drawLine(vec2(p1.x, p1.y), vec2(p2.x, p2.y), vec4(255, 255, 0, 255));
			}
			else
			{
				float color = 60 + 170 * (faceIndex % mesh.faces.size()) / mesh.faces.size();
				drawTriangle(p0, p1, p2, vec4(color, color, color, 255));
			}

			faceIndex++;
		}
	}
}

void Display::clear(vec4 color)
{
	SDL_FillRect(surface, 0, 0);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			depthBuffer[y * width + x] = 100; // todo: set to 1.1 because z is in range [-1; 1]
		}
	}
}

vec3 Display::project(vec3 coordinates, mat4 transformationMatrix)
{
	vec4 h = vec4::homogeneous(coordinates);
	vec4 clip = transformationMatrix * h;
	return vec3(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
}

void Display::drawPoint(vec2 coordinates, vec4 color)
{
	if (coordinates.x < 0 ||
		coordinates.y < 0 ||
		coordinates.x >= width ||
		coordinates.y >= height)
	{

		return;
	}

	Uint32* target_pixel;
	target_pixel = (Uint32 *)(surfacePixels +
		(int)coordinates.y * surfacePitch +
							  (int)coordinates.x * sizeof(*target_pixel));
	*target_pixel = SDL_MapRGB(surfacePixelFormat, color.x, color.y, color.z);
}

void Display::drawLine(vec2 p0, vec2 p1, vec4 color)
{
	int x0 = (int)p0.x;
	int y0 = (int)p0.y;
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		drawPoint(vec2(x0, y0), color);

		if ((x0 == x1) && (y0 == y1))
			break;

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void Display::drawTriangle(vec3 p1, vec3 p2, vec3 p3, vec4 color)
{
	vec3 temp;
	if (p1.y > p2.y)
	{
		temp = p1;
		p1 = p2;
		p2 = temp;
	}

	if (p2.y > p3.y)
	{
		temp = p2;
		p2 = p3;
		p3 = temp;
	}

	if (p1.y > p2.y)
	{
		temp = p1;
		p1 = p2;
		p2 = temp;
	}

	/*if ((p1.y == p2.y && p2.x > p1.x) ||
		(p2.y == p3.y && p2.x > p3.x) ||
		(p2.x - p1.x) / (p2.y - p1.x) > (p3.x - p1.x) / (p3.y - p1.x))*/

	if ((p2.x - p1.x) / (p2.y - p1.y) > (p3.x - p1.x) / (p3.y - p1.y))
	{
		// clockwise triangle
		for (int y = p1.y; y <= p3.y; y++)
		{
			if (y < p2.y)
				// step 1
				processScanLine(y, p1, p3, p1, p2, color);
			else
				// step 2
				processScanLine(y, p1, p3, p2, p3, color);
		}
	}
	else
	{
		// counter-clockwise triangle
		for (int y = p1.y; y <= p3.y; y++)
		{
			if (y < p2.y)
				// step 1
				processScanLine(y, p1, p2, p1, p3, color);
			else
				// step 2
				processScanLine(y, p2, p3, p1, p3, color);
		}
	}
}

void Display::processScanLine(int y, vec3 l1, vec3 l2, vec3 r1, vec3 r2, vec4 color)
{
	float gradient1 = l1.y == l2.y ? 1 : getGradient2(y, l1.y, l2.y);
	float gradient2 = r1.y == r2.y ? 1 : getGradient2(y, r1.y, r2.y);

	// intersection of y = y and l1l2 or l2.x when l1.y = l2.y
	int startX = linearInterpolation2(l1.x, l2.x, gradient1);
	// intersection of y = y and r1r2 or r2.x when r1.y = r2.y
	int endX = linearInterpolation2(r1.x, r2.x, gradient2);

	float startZ = linearInterpolation2(l1.z, l2.z, gradient1);
	float endZ = linearInterpolation2(r1.z, r2.z, gradient2);

	float gradientZ, z;
	int pixelIndex;
	for (int x = startX; x <= endX; x++)
	{
		gradientZ = getGradient2(x, startX, endX);
		z = linearInterpolation2(startZ, endZ, gradientZ);

		pixelIndex = y * width + x;

		if (z < depthBuffer[pixelIndex])
		{
			depthBuffer[pixelIndex] = z;
			drawPoint(vec2(x, y), color);
		}
	}
}

