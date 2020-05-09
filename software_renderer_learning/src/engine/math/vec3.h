#pragma once
#include <iostream>

struct vec3
{
	float x, y, z;
	vec3(const float& x, const float& y, const float& z);
	vec3();
	~vec3();

	float getCoordinate(const int i);
	void setCoordinate(const int i, const float coordinate);

	static vec3 normalize(const vec3 vector);
	static float dot(const vec3 left, const vec3 right);
	static vec3 cross(const vec3 left, const vec3 right);

	vec3& add(const vec3& other);
	vec3& sub(const vec3& other);
	vec3& mult(const vec3& other);
	vec3& div(const vec3& other);

	vec3& mult(const float& multiplier);

	friend vec3 operator+(vec3 left, const vec3& right);
	friend vec3 operator-(vec3 left, const vec3& right);
	friend vec3 operator*(vec3 left, const vec3& right);
	friend vec3 operator/(vec3 left, const vec3& right);

	friend vec3 operator*(vec3 left, const float& multiplier);

	vec3& operator+=(const vec3& other);
	vec3& operator-=(const vec3& other);
	vec3& operator*=(const vec3& other);
	vec3& operator/=(const vec3& other);

	vec3& operator*=(const float& multiplier);

	friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);
};
