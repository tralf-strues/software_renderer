#include "vec4.h"

vec4::vec4(const float& x, const float& y, const float& z, const float& w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4::vec4() : x(0), y(0), z(0), w(1)
{
}

vec4::~vec4()
{

}

float vec4::getCoordinate(const int i)
{
	switch (i)
	{
		case 0:
		return x;

		case 1:
		return y;

		case 2:
		return z;

		case 3:
		return w;

		default:
		return 0;
	}
}

void vec4::setCoordinate(const int i, const float coordinate)
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

		case 3:
		this->w = coordinate;
		break;
	}
}

vec3 vec4::toVec3(const vec4 vector)
{
	return vec3(vector.x / vector.w,
				vector.y / vector.w,
				vector.z / vector.w);
}

vec4 vec4::homogeneous(const vec3 vector)
{
	vec4 newVector(vector.x, vector.y, vector.z, 1);
	return newVector;
}

vec4 vec4::normalize(const vec4 vector)
{
	float length = sqrt(vector.x * vector.x + 
						vector.y * vector.y + 
						vector.z * vector.z + 
						vector.w * vector.w);

	vec4 newVector(vector.x / length,
				   vector.y / length, 
				   vector.z / length, 
				   vector.w / length);
	return newVector;
}

float vec4::dot(const vec4 left, const vec4 right)
{
	float product = 0;
	product += left.x * right.x;
	product += left.y * right.y;
	product += left.z * right.z;
	product += left.w * right.w;

	return product;
}

vec4& vec4::add(const vec4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

vec4& vec4::sub(const vec4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

vec4& vec4::mult(const vec4& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;

	return *this;
}

vec4& vec4::div(const vec4& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;

	return *this;
}

vec4& vec4::mult(const float& multiplier)
{
	x *= multiplier;
	y *= multiplier;
	z *= multiplier;
	w *= multiplier;

	return *this;
}

vec4 operator+(vec4 left, const vec4& right)
{
	return left.add(right);
}

vec4 operator-(vec4 left, const vec4& right)
{
	return left.sub(right);
}

vec4 operator*(vec4 left, const vec4& right)
{
	return left.mult(right);
}

vec4 operator/(vec4 left, const vec4& right)
{
	return left.div(right);
}

vec4 operator*(vec4 left, const float& multiplier)
{
	return left.mult(multiplier);
}

vec4& vec4::operator+=(const vec4& other)
{
	return add(other);
}

vec4& vec4::operator-=(const vec4& other)
{
	return sub(other);
}

vec4& vec4::operator*=(const vec4& other)
{
	return mult(other);
}

vec4& vec4::operator/=(const vec4& other)
{
	return div(other);
}

vec4& vec4::operator*=(const float& multiplier)
{
	return mult(multiplier);
}

std::ostream& operator<<(std::ostream& stream, const vec4& vector)
{
	stream << "Vector: (" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return stream;
}
