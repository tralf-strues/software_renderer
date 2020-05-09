#include "MathTools.h"
#include <SDL.h>

float degreesToRadians(float angle)
{
	return angle * M_PI / 180;
}

float radiansToDegrees(float angle)
{
	return angle * 180 / M_PI;
}

float getGradient2(float value, float start, float end)
{
	return (value - start) / (end - start);
}

float linearInterpolation2(float start, float end, float gradient)
{
	return (1 - gradient) * start + gradient * end;
}

vec2 inwardEdgeNormal(vec2 p0, vec2 p1)
{
	return vec2::normalize(vec2(-(p1.y - p0.y), p1.x - p0.x));
}

vec2 inwardEdgeNormal(vec3 p0, vec3 p1)
{
	return vec2::normalize(vec2(-(p1.y - p0.y), p1.x - p0.x));
}

vec3 triangleCenter(vec3 v0, vec3 v1, vec3 v2)
{
	return v0 * (1 / 3) + v1 * (1 / 3) + v2 * (1 / 3);
}

float signedArea2dTriangle(vec2 v0, vec2 v1, vec2 v2)
{
	return 1 / 2 * ((v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y));
}