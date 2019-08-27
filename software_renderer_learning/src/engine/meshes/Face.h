#pragma once

#include "../math/engineMath.h"
#include <vector>

struct Face
{
	Face();
	~Face();

	vec3 vertexIndices;
	vec3 textureIndices;
	vec3 normalIndices;
};