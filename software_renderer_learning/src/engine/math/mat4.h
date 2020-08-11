#pragma once
#include "vec3.h"
#include "vec4.h"

#define MATRIX_SIZE 4

struct mat4
{
	float elements[MATRIX_SIZE][MATRIX_SIZE];
	mat4();
	~mat4();

	static mat4& identity(float diagonal);
	static mat4& orthographic(float left, float right, float bottom, float top, float near, float far);
	static mat4& perspective(float left, float right, float bottom, float top, float near, float far);

	/*
		fovy   - specifies the field of view angle, in radians, in the y direction
		aspect - screen.width / scren.height;
		near  - specifies the distance from the viewer to the near clipping plane (always positive)
		far   - specifies the distance from the viewer to the far clipping plane (always positive)
	*/
	static mat4& perspectiveFOV(float fovy, float aspect, float near, float far);
	static mat4& translation(const vec3& translate);
	static mat4& rotationPitchYawRoll(const vec3& angles);
	static mat4& lootAtDirectionPoints(const vec3& from, const vec3& to, const vec3& tmp = vec3(0, 1, 0));
	static mat4& lootAtDirectionVector(const vec3& from, const vec3& forward, const vec3& tmp = vec3(0, 1, 0));
	

	mat4 mult(const mat4& other);
	vec4 mult(vec4& vector);
	friend mat4 operator*(mat4 left, const mat4& right);
	friend vec4 operator*(mat4 matrix, vec4 vector);
};
