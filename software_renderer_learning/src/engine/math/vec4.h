#pragma once
#include <iostream>
#include <SDL.h>
#include "vec3.h"

struct vec4
{
	float x, y, z, w;
	vec4(const float& x, const float& y, const float& z, const float& w);
	vec4();
	~vec4();

	float getCoordinate(const int i);
	void setCoordinate(const int i, const float coordinate);

	static vec4 homogeneous(const vec3 vector);
	static vec3 toVec3(const vec4 vector);
	static vec4 normalize(const vec4 vector);
	static float length(const vec4 vector);
	static float length3d(const vec4 vector);
	static float dot(const vec4 left, const vec4 right);

	vec4& add(const vec4& other);
	vec4& sub(const vec4& other);
	vec4& mult(const vec4& other);
	vec4& div(const vec4& other);

	vec4& mult(const float& multiplier);

	friend vec4 operator+(vec4 left, const vec4& right);
	friend vec4 operator-(vec4 left, const vec4& right);
	friend vec4 operator*(vec4 left, const vec4& right);
	friend vec4 operator/(vec4 left, const vec4& right);

	friend bool operator==(vec4 left, const vec4& right);

	friend vec4 operator*(vec4 left, const float& multiplier);

	vec4& operator+=(const vec4& other);
	vec4& operator-=(const vec4& other);
	vec4& operator*=(const vec4& other);
	vec4& operator/=(const vec4& other);

	vec4& operator*=(const float& multiplier);

	friend std::ostream& operator<<(std::ostream& stream, const vec4& vector);
};
