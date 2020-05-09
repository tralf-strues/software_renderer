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
		   vec3 worldCoordinates);
	Vertex(vec3 normal, vec3 coordinates);
	Vertex();
	~Vertex();

	vec3 normal;
	vec3 coordinates;
	vec3 worldCoordinates;
	vec3 projectedCoordinates;
	vec2 screenCoordinates;
};

