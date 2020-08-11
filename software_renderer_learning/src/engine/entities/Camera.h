#pragma once
#include "../math/engineMath.h"

#define ANGLE_Y_LIMIT 1.56

struct Camera 
{
	vec3 position;
	//vec3 to;
	vec3 forward;
	float angleY; // angle between forward and its projection on xz plane

	Camera(vec3 position, vec3 forward);
	//Camera(vec3 position, vec3 to, float angleY);
	~Camera();

};

