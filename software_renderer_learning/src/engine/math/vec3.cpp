#include "vec3.h"

vec3::vec3(const float& x, const float& y, const float& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::vec3() : x(0), y(0), z(0)
{
}

vec3::~vec3() 
{

}

float vec3::getCoordinate(const int i)
{
	switch (i)
	{
		case 0:
		return x;

		case 1:
		return y;

		case 2:
		return z;

		default:
		return 0;
	}
}

void vec3::setCoordinate(const int i, const float coordinate)
{
	switch (i)
	{
		case 0:
		this->x = coordinate;
		break;

		case 1:
		this->y = coordinate;
		break;

		case 2:
		this->z = coordinate;
		break;
	}
}

vec3 vec3::normalize(const vec3& vector)
{
	float length = sqrt(vector.x * vector.x +
						vector.y * vector.y +
						vector.z * vector.z);

	vec3 newVector(vector.x / length,
				   vector.y / length,
				   vector.z / length);
	return newVector;
}

float vec3::length(const vec3& vector)
{
	return sqrt(vector.x * vector.x +
				vector.y * vector.y +
				vector.z * vector.z);
}

float vec3::dot(const vec3& left, const vec3& right)
{
	float product = 0;
	product += left.x * right.x;
	product += left.y * right.y;
	product += left.z * right.z;

	return product;
}

vec3 vec3::cross(const vec3& left, const vec3& right)
{
	vec3 product;
	product.x = left.y * right.z - left.z * right.y;
	product.y = left.z * right.x - left.x * right.z;
	product.z = left.x * right.y - left.y * right.x;

	return product;
}

vec3& vec3::add(const vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

vec3& vec3::sub(const vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

vec3& vec3::mult(const vec3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

vec3& vec3::div(const vec3& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

vec3& vec3::mult(const float& multiplier)
{
	x *= multiplier;
	y *= multiplier;
	z *= multiplier;

	return *this;
}

vec3 operator+(vec3 left, const vec3& right)
{
	return left.add(right);
}

vec3 operator-(vec3 left, const vec3& right)
{
	return left.sub(right);
}

vec3 operator*(vec3 left, const vec3& right)
{
	return left.mult(right);
}

vec3 operator/(vec3 left, const vec3& right)
{
	return left.div(right);
}

vec3 operator*(vec3 left, const float& multiplier)
{
	return left.mult(multiplier);
}

bool operator==(vec3 left, const vec3& right)
{
	if (left.x == right.x &&
		left.y == right.y &&
		left.z == right.z)
		return true;
	else
		return false;
}

vec3& vec3::operator+=(const vec3& other)
{
	return add(other);
}

vec3& vec3::operator-=(const vec3& other)
{
	return sub(other);
}

vec3& vec3::operator*=(const vec3& other)
{
	return mult(other);
}

vec3& vec3::operator/=(const vec3& other)
{
	return div(other);
}

vec3& vec3::operator*=(const float& multiplier)
{
	return mult(multiplier);
}

std::ostream& operator<<(std::ostream& stream, const vec3& vector)
{
	stream << "Vector: (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}
