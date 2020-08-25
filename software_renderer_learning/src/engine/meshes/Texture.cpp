#include "Texture.h"
#include <math.h>

Texture::Texture(std::string name, int width, int height, std::vector<vec4> pixels) :
	name(name), width(width), height(height), pixels(pixels)
{
}

vec4 Texture::getPixel(vec2 uv)
{
	//if (uv == vec2())
	int u = ((int)std::round(uv.x)) % width;
	int v = (height - (((int)std::round(uv.y)) % height)) % height;

	return pixels[v * width + u];
}