#include "Texture.h"

Texture::Texture(std::string name, int width, int height, std::vector<vec4> pixels):
	name(name), width(width), height(height), pixels(pixels)
{
}

vec4 Texture::getPixel(vec2 uv)
{
	return pixels[uv.y * width + uv.x];
}