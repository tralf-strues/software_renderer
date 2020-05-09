#pragma once
#include "engineMath.h"

float degreesToRadians(float angle);
float radiansToDegrees(float angle);

/* value = (1 - gradient) * start + gradient * end
or 
value = start + gradient * (end - start)

0->start
1->end */
float getGradient2(float value, float start, float end);
float linearInterpolation2(float start, float end, float gradient);

vec2 inwardEdgeNormal(vec2 p0, vec2 p1); 
vec3 triangleCenter(vec3 v0, vec3 v1, vec3 v2);
float signedArea2dTriangle(vec2 v0, vec2 v1, vec2 v2);