#pragma once

#include "../math/vec3.h"
#include "../math/vec2.h"

class Vertex
{
public:
	Vertex(vec3 normal, 
		   vec3 coordinates, 
		   vec3 projectedCoordinates, 
		   vec2 screenCoordinates, 
		   vec3 eyeCoordinates);
	Vertex(vec3 normal, vec3 coordinates);
	Vertex();
	~Vertex();

	vec3 normal;
	vec3 coordinates;
	vec3 eyeCoordinates;
	vec3 projectedCoordinates;
	vec2 screenCoordinates;
};

