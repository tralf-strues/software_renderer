#include "Vertex.h"

Vertex::Vertex(vec3 normal, 
			   vec3 coordinates,
			   vec3 projectedCoordinates, 
			   vec2 screenCoordinates, 
			   vec3 worldCoordinates) :
	normal(normal), 
	coordinates(coordinates),
	projectedCoordinates(projectedCoordinates),
	screenCoordinates(screenCoordinates),
	worldCoordinates(worldCoordinates)
{
}

Vertex::Vertex(vec3 normal, vec3 coordinates) :
	normal(normal),
	coordinates(coordinates)
{
}

Vertex::Vertex() :
	normal(vec3(0, 1, 0))
{
}


Vertex::~Vertex()
{
}