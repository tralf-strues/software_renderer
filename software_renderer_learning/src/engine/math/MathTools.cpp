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