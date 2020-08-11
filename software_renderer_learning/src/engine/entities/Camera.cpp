#include "Camera.h"

//Camera::Camera(vec3 position, vec3 to, float angleY) : 
//	position(position), 
//	to(to), 
//	angleY(angleY)
//{
//}

Camera::Camera(vec3 position, vec3 forward) :
	position(position),
	forward(vec3::normalize(forward)),
	angleY(0) // todo
{
	
}

Camera::~Camera()
{
}
