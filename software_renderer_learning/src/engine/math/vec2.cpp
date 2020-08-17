#include "vec2.h"

vec2::vec2(const float& x, const float& y)
{
	this->x = x;
	this->y = y;
}

vec2::vec2() : x(0), y(0)
{
}

vec2::~vec2()
{
}

float vec2::getCoordinate(const int i)
{
	switch (i)
	{
		case 0:
		return x;

		case 1:
		return y;

		default:
		return 0;
	}
}

void vec2::setCoordinate(const int i, const float coordinate)
{
	switch (i)
	{
		case 0:
		this->x = coordinate;
		break;

		case 1:
		this->y = coordinate;
		break;
	}
}

vec2 vec2::normalize(const vec2 vector)
{
	float length = sqrt(vector.x * vector.x +
						vector.y * vector.y);

	vec2 newVector(vector.x / length,
				   vector.y / length);
	return newVector;
}

float vec2::length(const vec2 vector)
{
	return sqrt(vector.x * vector.x +
				vector.y * vector.y);
}

float vec2::dot(const vec2 left, const vec2 right)
{
	float product = 0;
	product += left.x * right.x;
	product += left.y * right.y;

	return product;
}

vec2& vec2::add(const vec2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

vec2& vec2::sub(const vec2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

vec2& vec2::mult(const vec2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

vec2& vec2::div(const vec2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

vec2& vec2::mult(const float& multiplier)
{
	x *= multiplier;
	y *= multiplier;

	return *this;
}

vec2 operator+(vec2 left, const vec2& right)
{
	return left.add(right);
}

vec2 operator-(vec2 left, const vec2& right)
{
	return left.sub(right);
}

vec2 operator*(vec2 left, const vec2& right)
{
	return left.mult(right);
}

vec2 operator/(vec2 left, const vec2& right)
{
	return left.div(right);
}

bool operator==(vec2 left, const vec2& right)
{
	if (left.x == right.x &&
		left.y == right.y)
		return true;
	else
		return false;
}

vec2 operator*(vec2 left, const float& multiplier)
{
	return left.mult(multiplier);
}

vec2& vec2::operator+=(const vec2& other)
{
	return add(other);
}

vec2& vec2::operator-=(const vec2& other)
{
	return sub(other);
}

vec2& vec2::operator*=(const vec2& other)
{
	return mult(other);
}

vec2& vec2::operator/=(const vec2& other)
{
	return div(other);
}

vec2& vec2::operator*=(const float& multiplier)
{
	return mult(multiplier);
}


std::ostream& operator<<(std::ostream& stream, const vec2& vector)
{
	stream << "Vector: (" << vector.x << ", " << vector.y << ")";
	return stream;
}
