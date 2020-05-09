#pragma once
#include "../math/engineMath.h"

struct Camera 
{
	vec3 position;
	vec3 to;

	// pitch - around z; yaw - around y; roll - around x
	Camera(vec3 position, vec3 to);
	~Camera();

};

