#pragma once

#include "../math/engineMath.h"

struct LightSource
{
	vec4 position;
	vec3 eyeCoordinates;
	vec4 color;
	float brightness; 

	LightSource(vec4 position, vec4 color, float brightness);
};

