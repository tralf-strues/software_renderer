#pragma once

#include "../math/engineMath.h"

struct LightSource
{
	vec3 position;
	vec4 color;
	float brightness; 

	LightSource(vec3 position, vec4 color, float brightness);
};

