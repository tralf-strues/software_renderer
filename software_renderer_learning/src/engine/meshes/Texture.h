#pragma once

#include <string>
#include <vector>
#include "../math/vec4.h"
#include "../math/vec2.h"

struct Texture
{
	Texture(std::string name, int width, int height, std::vector<vec4> pixels);

	std::string name;
	int width;
	int height;
	std::vector<vec4> pixels;

	vec4 getPixel(vec2 uv);
};