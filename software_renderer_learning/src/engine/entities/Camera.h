#pragma once
#include "../math/engineMath.h"

struct Camera 
{
	vec3 position;
	vec3 target;

	Camera(vec3 position, vec3 target);
	~Camera();

};

