#pragma once
#include <iostream>

struct vec2
{
	float x, y;
	vec2(const float&x, const float&y);
	vec2();
	~vec2();

	float getCoordinate(const int i);
	void setCoordinate(const int i, const float coordinate);

	static vec2 normalize(const vec2 vector);
	static float length(const vec2 vector);
	static float dot(const vec2 left, const vec2 right);

	vec2& add(const vec2& other);
	vec2& sub(const vec2& other);
	vec2& mult(const vec2& other);
	vec2& div(const vec2& other);

	vec2& mult(const float& multiplier);

	friend vec2 operator+(vec2 left, const vec2& right);
	friend vec2 operator-(vec2 left, const vec2& right);
	friend vec2 operator*(vec2 left, const vec2& right);
	friend vec2 operator/(vec2 left, const vec2& right);

	friend vec2 operator*(vec2 left, const float& multiplier);

	vec2& operator+=(const vec2& other);
	vec2& operator-=(const vec2& other);
	vec2& operator*=(const vec2& other);
	vec2& operator/=(const vec2& other);

	vec2& operator*=(const float& multiplier);

	friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);
};
