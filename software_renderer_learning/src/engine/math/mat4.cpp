#include <math.h>
#include "mat4.h"

mat4::mat4()
{
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			elements[i][j] = 0.0f;
}

mat4::~mat4()
{

}

mat4& mat4::identity(float diagonal)
{
	mat4 identity;

	identity.elements[0][0] = diagonal;
	identity.elements[1][1] = diagonal;
	identity.elements[2][2] = diagonal;
	identity.elements[3][3] = diagonal;

	return identity;
}

mat4& mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
{
	mat4 ortho;

	ortho.elements[0][0] = 2 / (right - left);
	ortho.elements[0][1] = 0;
	ortho.elements[0][2] = 0;
	ortho.elements[0][3] = - (right + left) / (right - left);

	ortho.elements[1][0] = 0;
	ortho.elements[1][1] = 2 / (top - bottom);
	ortho.elements[1][2] = 0;
	ortho.elements[1][3] = - (top + bottom) / (top - bottom);
			   
	ortho.elements[2][0] = 0;
	ortho.elements[2][1] = 0;
	ortho.elements[2][2] = -2 / (far - near);
	ortho.elements[2][3] = - (far + near) / (far - near);

	ortho.elements[3][0] = 0;
	ortho.elements[3][1] = 0;
	ortho.elements[3][2] = 0;
	ortho.elements[3][3] = 1;

	return ortho;
}

mat4& mat4::perspective(float left, float right, float bottom, float top, float near, float far)
{
	mat4 perspective;

	perspective.elements[0][0] = 2 * near / (right - left);
	perspective.elements[0][1] = 0;
	perspective.elements[0][2] = (right + left) / (right - left);
	perspective.elements[0][3] = 0;

	perspective.elements[1][0] = 0;
	perspective.elements[1][1] = 2 * near / (top - bottom);
	perspective.elements[1][2] = (top + bottom) / (top - bottom);
	perspective.elements[1][3] = 0;

	perspective.elements[2][0] = 0;
	perspective.elements[2][1] = 0;
	perspective.elements[2][2] = - (far + near) / (far - near);
	perspective.elements[2][3] = - 2 * far * near / (far - near);

	perspective.elements[3][0] = 0;
	perspective.elements[3][1] = 0;
	perspective.elements[3][2] = -1;
	perspective.elements[3][3] = 0;

	return perspective;
}

mat4& mat4::perspectiveFOV(float fovy, float aspect, float near, float far)
{
	float top = near * std::tan(fovy / 2);
	float bottom = -top;
	float right = aspect * top;
	float left = -right;

	return perspective(left, right, bottom, top, near, far);
}

mat4& mat4::translation(const vec3& translate)
{
	mat4 result = identity(1.0f);

	result.elements[0][3] = translate.x;
	result.elements[1][3] = translate.y;
	result.elements[2][3] = translate.z;

	return result;
}

mat4& mat4::rotationPitchYawRoll(const vec3& angles)
{
	float sinph = std::sin(angles.x), cosph = std::cos(angles.x),
		sinth = std::sin(angles.y), costh = std::cos(angles.y),
		sinps = std::sin(angles.z), cosps = std::cos(angles.z);

	mat4 rotation;
	rotation.elements[0][0] = costh * cosph;
	rotation.elements[0][1] = -costh * sinph;
	rotation.elements[0][2] = sinth;
	rotation.elements[0][3] = 0;

	rotation.elements[1][0] = sinps * sinth * cosph + cosps * sinph;
	rotation.elements[1][1] = cosps * cosph - sinps * sinth * sinph;
	rotation.elements[1][2] = -sinps * costh;
	rotation.elements[1][3] = 0;

	rotation.elements[2][0] = sinps * sinph - cosps * sinth * cosph;
	rotation.elements[2][1] = cosps * sinth * sinph + sinps * cosph;
	rotation.elements[2][2] = cosps * costh;
	rotation.elements[2][3] = 0;

	rotation.elements[3][0] = 0;
	rotation.elements[3][1] = 0;
	rotation.elements[3][2] = 0;
	rotation.elements[3][3] = 1;

	return rotation;
}

mat4& mat4::lootAtDirectionPoints(const vec3& from, const vec3& to, const vec3& tmp)
{
	vec3 f(vec3::normalize(from - to));				// forward
	vec3 r = vec3::cross(vec3::normalize(tmp), f);	// right
	vec3 u = vec3::cross(f, r);						// up

	mat4 viewMatrix;
	viewMatrix.elements[0][0] = r.x;
	viewMatrix.elements[0][1] = r.y;
	viewMatrix.elements[0][2] = r.z;
	viewMatrix.elements[0][3] = -vec3::dot(r, from);

	viewMatrix.elements[1][0] = u.x;
	viewMatrix.elements[1][1] = u.y;
	viewMatrix.elements[1][2] = u.z;
	viewMatrix.elements[1][3] = -vec3::dot(u, from);

	viewMatrix.elements[2][0] = f.x;
	viewMatrix.elements[2][1] = f.y;
	viewMatrix.elements[2][2] = f.z;
	viewMatrix.elements[2][3] = -vec3::dot(f, from);

	viewMatrix.elements[3][0] = 0;
	viewMatrix.elements[3][1] = 0;
	viewMatrix.elements[3][2] = 0;
	viewMatrix.elements[3][3] = 1;

	return viewMatrix;
}

mat4& mat4::lootAtDirectionVector(const vec3& from, const vec3& forward, const vec3& tmp)
{
	vec3 f(vec3::normalize(forward));				// forward
	vec3 r = vec3::cross(vec3::normalize(tmp), f);	// right
	vec3 u = vec3::cross(f, r);						// up

	mat4 viewMatrix;
	viewMatrix.elements[0][0] = r.x;
	viewMatrix.elements[0][1] = r.y;
	viewMatrix.elements[0][2] = r.z;
	viewMatrix.elements[0][3] = -vec3::dot(r, from);

	viewMatrix.elements[1][0] = u.x;
	viewMatrix.elements[1][1] = u.y;
	viewMatrix.elements[1][2] = u.z;
	viewMatrix.elements[1][3] = -vec3::dot(u, from);

	viewMatrix.elements[2][0] = f.x;
	viewMatrix.elements[2][1] = f.y;
	viewMatrix.elements[2][2] = f.z;
	viewMatrix.elements[2][3] = -vec3::dot(f, from);

	viewMatrix.elements[3][0] = 0;
	viewMatrix.elements[3][1] = 0;
	viewMatrix.elements[3][2] = 0;
	viewMatrix.elements[3][3] = 1;

	return viewMatrix;
}

mat4 mat4::mult(const mat4& other)
{
	mat4 newMatrix;
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			newMatrix.elements[i][j] = 0;

			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				newMatrix.elements[i][j] += elements[i][k] * other.elements[k][j];
			}
		}
	}

	return newMatrix;
}

vec4 mat4::mult(vec4& vector)
{
	vec4 newVector;
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		newVector.setCoordinate(i, 0);

		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			newVector.setCoordinate(i, newVector.getCoordinate(i) + 
									elements[i][j] * vector.getCoordinate(j));
		}
	}

	return newVector;
}

mat4 operator*(mat4 left, const mat4& right)
{
	return left.mult(right);
}

vec4 operator*(mat4 matrix, vec4 vector)
{
	return matrix.mult(vector);
}

