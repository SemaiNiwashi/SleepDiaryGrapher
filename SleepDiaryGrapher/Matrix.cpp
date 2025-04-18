#include "Matrix.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#pragma region //mat3
mat3::mat3()
{
	data[0] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;
	data[4] = 1.0f;
	data[5] = 0.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;
	data[8] = 1.0f;
}
mat3::mat3(float Data[9])
{
	data[0] = Data[0];
	data[1] = Data[1];
	data[2] = Data[2];
	data[3] = Data[3];
	data[4] = Data[4];
	data[5] = Data[5];
	data[6] = Data[6];
	data[7] = Data[7];
	data[8] = Data[8];
}
mat3::mat3(mat4 dataMat)
{
	data[0] = dataMat.data[0];
	data[1] = dataMat.data[1];
	data[2] = dataMat.data[2];
	data[3] = dataMat.data[4];
	data[4] = dataMat.data[5];
	data[5] = dataMat.data[6];
	data[6] = dataMat.data[8];
	data[7] = dataMat.data[9];
	data[8] = dataMat.data[10];
}
mat3::mat3(float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8)
{
	data[0] = d0;
	data[1] = d1;
	data[2] = d2;
	data[3] = d3;
	data[4] = d4;
	data[5] = d5;
	data[6] = d6;
	data[7] = d7;
	data[8] = d8;
}
mat3 &mat3::operator=(const mat3 &M)
{
	data[0] = M.data[0];
	data[1] = M.data[1];
	data[2] = M.data[2];
	data[3] = M.data[3];
	data[4] = M.data[4];
	data[5] = M.data[5];
	data[6] = M.data[6];
	data[7] = M.data[7];
	data[8] = M.data[8];
	return *this;
}
mat3 &mat3::operator*=(float scalar)
{
	data[0] *= scalar;
	data[1] *= scalar;
	data[2] *= scalar;
	data[3] *= scalar;
	data[4] *= scalar;
	data[5] *= scalar;
	data[6] *= scalar;
	data[7] *= scalar;
	data[8] *= scalar;
	return *this;
}
mat3 &mat3::operator*=(const mat3 &M)
{
	mat3 temp;
	temp.data[0] = M.data[0] * data[0] + M.data[3] * data[1] + M.data[6] * data[2];
	temp.data[1] = M.data[1] * data[0] + M.data[4] * data[1] + M.data[7] * data[2];
	temp.data[2] = M.data[2] * data[0] + M.data[5] * data[1] + M.data[8] * data[2];
	temp.data[3] = M.data[0] * data[3] + M.data[3] * data[4] + M.data[6] * data[5];
	temp.data[4] = M.data[1] * data[3] + M.data[4] * data[4] + M.data[7] * data[5];
	temp.data[5] = M.data[2] * data[3] + M.data[5] * data[4] + M.data[8] * data[5];
	temp.data[6] = M.data[0] * data[6] + M.data[3] * data[7] + M.data[6] * data[8];
	temp.data[7] = M.data[1] * data[6] + M.data[4] * data[7] + M.data[7] * data[8];
	temp.data[8] = M.data[2] * data[6] + M.data[5] * data[7] + M.data[8] * data[8];

	data[0] = temp.data[0];
	data[1] = temp.data[1];
	data[2] = temp.data[2];
	data[3] = temp.data[3];
	data[4] = temp.data[4];
	data[5] = temp.data[5];
	data[6] = temp.data[6];
	data[7] = temp.data[7];
	data[8] = temp.data[8];
	return *this;
}
mat3 &mat3::operator/=(float scalar)
{
	data[0] /= scalar;
	data[1] /= scalar;
	data[2] /= scalar;
	data[3] /= scalar;
	data[4] /= scalar;
	data[5] /= scalar;
	data[6] /= scalar;
	data[7] /= scalar;
	data[8] /= scalar;
	return *this;
}
mat3 &mat3::operator+=(const mat3 &M)
{
	data[0] += M.data[0];
	data[1] += M.data[1];
	data[2] += M.data[2];
	data[3] += M.data[3];
	data[4] += M.data[4];
	data[5] += M.data[5];
	data[6] += M.data[6];
	data[7] += M.data[7];
	data[8] += M.data[8];
	return *this;
}
mat3 &mat3::operator-=(const mat3 &M)
{
	data[0] -= M.data[0];
	data[1] -= M.data[1];
	data[2] -= M.data[2];
	data[3] -= M.data[3];
	data[4] -= M.data[4];
	data[5] -= M.data[5];
	data[6] -= M.data[6];
	data[7] -= M.data[7];
	data[8] -= M.data[8];
	return *this;
}
mat3 mat3::operator*(float scalar) const
{
	mat3 temp;
	temp.data[0] *= scalar;
	temp.data[1] *= scalar;
	temp.data[2] *= scalar;
	temp.data[3] *= scalar;
	temp.data[4] *= scalar;
	temp.data[5] *= scalar;
	temp.data[6] *= scalar;
	temp.data[7] *= scalar;
	temp.data[8] *= scalar;
	return temp;
}
vec2 mat3::operator*(const vec2 &M) const
{
	vec3 asVec3 = { M.x,M.y,0.0f };
	vec3 temp;
	temp.x = asVec3.x * data[0] + asVec3.y * data[1] + asVec3.z * data[2];
	temp.y = asVec3.x * data[3] + asVec3.y * data[4] + asVec3.z * data[5];
	temp.z = asVec3.x * data[6] + asVec3.y * data[7] + asVec3.z * data[8];
	return vec2(temp.x,temp.y);
}
vec3 mat3::operator*(const vec3 &M) const
{
	vec3 temp;
	temp.x = M.x * data[0] + M.y * data[1] + M.z * data[2];
	temp.y = M.x * data[3] + M.y * data[4] + M.z * data[5];
	temp.z = M.x * data[6] + M.y * data[7] + M.z * data[8];
	return temp;
}
mat3 mat3::operator*(const mat3 &M) const
{
	mat3 temp;
	temp.data[0] = M.data[0] * data[0] + M.data[3] * data[1] + M.data[6] * data[2];
	temp.data[1] = M.data[1] * data[0] + M.data[4] * data[1] + M.data[7] * data[2];
	temp.data[2] = M.data[2] * data[0] + M.data[5] * data[1] + M.data[8] * data[2];
	temp.data[3] = M.data[0] * data[3] + M.data[3] * data[4] + M.data[6] * data[5];
	temp.data[4] = M.data[1] * data[3] + M.data[4] * data[4] + M.data[7] * data[5];
	temp.data[5] = M.data[2] * data[3] + M.data[5] * data[4] + M.data[8] * data[5];
	temp.data[6] = M.data[0] * data[6] + M.data[3] * data[7] + M.data[6] * data[8];
	temp.data[7] = M.data[1] * data[6] + M.data[4] * data[7] + M.data[7] * data[8];
	temp.data[8] = M.data[2] * data[6] + M.data[5] * data[7] + M.data[8] * data[8];
	return temp;
}
mat3 mat3::operator/(float scalar) const
{
	mat3 temp;
	temp.data[0] = data[0] / scalar;
	temp.data[1] = data[1] / scalar;
	temp.data[2] = data[2] / scalar;
	temp.data[3] = data[3] / scalar;
	temp.data[4] = data[4] / scalar;
	temp.data[5] = data[5] / scalar;
	temp.data[6] = data[6] / scalar;
	temp.data[7] = data[7] / scalar;
	temp.data[8] = data[8] / scalar;
	return temp;
}
mat3 mat3::operator+(const mat3 &M) const
{
	mat3 temp;
	temp.data[0] = data[0] + M.data[0];
	temp.data[1] = data[1] + M.data[1];
	temp.data[2] = data[2] + M.data[2];
	temp.data[3] = data[3] + M.data[3];
	temp.data[4] = data[4] + M.data[4];
	temp.data[5] = data[5] + M.data[5];
	temp.data[6] = data[6] + M.data[6];
	temp.data[7] = data[7] + M.data[7];
	temp.data[8] = data[8] + M.data[8];
	return temp;
}
mat3 mat3::operator-(const mat3 &M) const
{
	mat3 temp;
	temp.data[0] = data[0] - M.data[0];
	temp.data[1] = data[1] - M.data[1];
	temp.data[2] = data[2] - M.data[2];
	temp.data[3] = data[3] - M.data[3];
	temp.data[4] = data[4] - M.data[4];
	temp.data[5] = data[5] - M.data[5];
	temp.data[6] = data[6] - M.data[6];
	temp.data[7] = data[7] - M.data[7];
	temp.data[8] = data[8] - M.data[8];
	return temp;
}
mat3 mat3::operator-() const
{
	mat3 temp;
	temp.data[0] = -data[0];
	temp.data[1] = -data[1];
	temp.data[2] = -data[2];
	temp.data[3] = -data[3];
	temp.data[4] = -data[4];
	temp.data[5] = -data[5];
	temp.data[6] = -data[6];
	temp.data[7] = -data[7];
	temp.data[8] = -data[8];
	return temp;
}
bool mat3::operator==(const mat3 &M) const
{
	return (data[0] == M.data[0] && data[1] == M.data[1] && data[2] == M.data[2] && data[3] == M.data[3] && data[4] == M.data[4] && data[5] == M.data[5] && data[6] == M.data[6] && data[7] == M.data[7] && data[8] == M.data[8]);
}
void mat3::RotateX(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat3 rotation(1.0f, 0.0f, 0.0f, 0.0f, cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees));
	*this = rotation * (*this);
}
void mat3::RotateY(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat3 rotation(cos(degrees), 0.0f, sin(degrees), 0.0f, 1.0f, 0.0f, -sin(degrees), 0.0f, cos(degrees));
	*this = rotation * (*this);
}
mat3 mat3::getRotateY(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat3 rotation(cos(degrees), 0.0f, sin(degrees), 0.0f, 1.0f, 0.0f, -sin(degrees), 0.0f, cos(degrees));
	return *this * rotation;
}
void mat3::RotateZ(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat3 rotation(cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * (*this);
}
void mat3::Rotate(float x, float y, float z, float degrees)
{
	mat3 rotationX(1.0f, 0.0f, 0.0f, 0.0f, cos(degrees*x), -sin(degrees*x), 0.0f, sin(degrees*x), cos(degrees*x));
	mat3 rotationY(cos(degrees*y), 0.0f, sin(degrees*y), 0.0f, 1.0f, 0.0f, -sin(degrees*y), 0.0f, cos(degrees*y));
	mat3 rotationZ(cos(degrees*z), -sin(degrees*z), 0.0f, sin(degrees*z), cos(degrees*z), 0.0f, 0.0f, 0.0f, 1.0f);

	mat3 rotationMatrixZY;
	mat3 rotationMatrixZYX;

	rotationMatrixZY = rotationZ * rotationY;

	rotationMatrixZYX = rotationMatrixZY * rotationX;

	/*dir.x = rotationMatrixZYX[0] * dir.x + rotationMatrixZYX[1] * dir.y + rotationMatrixZYX[2] * dir.z;
	dir.y = rotationMatrixZYX[3] * dir.x + rotationMatrixZYX[4] * dir.y + rotationMatrixZYX[5] * dir.z;
	dir.z = rotationMatrixZYX[6] * dir.x + rotationMatrixZYX[7] * dir.y + rotationMatrixZYX[8] * dir.z;*/
}
void mat3::Scale(vec3 scaleFactor)
{
	*this = mat3(scaleFactor.x, 0.0f, 0.0f, 0.0f, scaleFactor.y, 0.0f, 0.0f, 0.0f, scaleFactor.z) * (*this);
}
mat3 mat3::getScale(vec3 scaleFactor)
{
	return mat3(scaleFactor.x, 0.0f, 0.0f, 0.0f, scaleFactor.y, 0.0f, 0.0f, 0.0f, scaleFactor.z) * (*this);
}
void mat3::Transpose()
{
	mat3 temp = *this;
	data[0] = temp.data[0];
	data[1] = temp.data[3];
	data[2] = temp.data[6];
	data[3] = temp.data[1];
	data[4] = temp.data[4];
	data[5] = temp.data[7];
	data[6] = temp.data[2];
	data[7] = temp.data[5];
	data[8] = temp.data[8];
}
vec3 mat3::getColumn(int colNum)
{
	if (colNum == 0)
	{
		return{ data[0], data[3], data[6] };
	}
	else if (colNum == 1)
	{
		return{ data[1], data[4], data[7] };
	}
	else if (colNum == 2)
	{
		return{ data[2], data[5], data[8] };
	}
	return{ 0, 0, 0 };

}
void mat3::output()
{
	std::cout << data[0] << " " << data[1] << " " << data[2] << "\n" << data[3] << " " << data[4] << " " << data[5] << "\n" << data[6] << " " << data[7] << " " << data[8] << std::endl;
}
void mat3::setIdent()
{
	data[0] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;
	data[4] = 1.0f;
	data[5] = 0.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;
	data[8] = 1.0f;
}
#pragma endregion

#pragma region //mat4
mat4::mat4()
{
	data[0] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;
	data[4] = 0.0f;
	data[5] = 1.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;
	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = 1.0f;
	data[11] = 0.0f;
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
}
mat4::mat4(float Data[16])
{
	data[0] = Data[0];
	data[1] = Data[1];
	data[2] = Data[2];
	data[3] = Data[3];
	data[4] = Data[4];
	data[5] = Data[5];
	data[6] = Data[6];
	data[7] = Data[7];
	data[8] = Data[8];
	data[9] = Data[9];
	data[10] = Data[10];
	data[11] = Data[11];
	data[12] = Data[12];
	data[13] = Data[13];
	data[14] = Data[14];
	data[15] = Data[15];
}
mat4::mat4(float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, float d9, float d10, float d11, float d12, float d13, float d14, float d15)
{
	data[0] = d0;
	data[1] = d1;
	data[2] = d2;
	data[3] = d3;
	data[4] = d4;
	data[5] = d5;
	data[6] = d6;
	data[7] = d7;
	data[8] = d8;
	data[9] = d9;
	data[10] = d10;
	data[11] = d11;
	data[12] = d12;
	data[13] = d13;
	data[14] = d14;
	data[15] = d15;
}
mat4::mat4(mat3 rotation, vec3 position)
{
	data[0] = rotation.data[0];
	data[1] = rotation.data[1];
	data[2] = rotation.data[2];
	data[3] = position.x;
	data[4] = rotation.data[3];
	data[5] = rotation.data[4];
	data[6] = rotation.data[5];
	data[7] = position.y;
	data[8] = rotation.data[6];
	data[9] = rotation.data[7];
	data[10] = rotation.data[8];
	data[11] = position.z;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}
mat4 &mat4::operator=(const mat4 &M)
{
	data[0] = M.data[0];
	data[1] = M.data[1];
	data[2] = M.data[2];
	data[3] = M.data[3];
	data[4] = M.data[4];
	data[5] = M.data[5];
	data[6] = M.data[6];
	data[7] = M.data[7];
	data[8] = M.data[8];
	data[9] = M.data[9];
	data[10] = M.data[10];
	data[11] = M.data[11];
	data[12] = M.data[12];
	data[13] = M.data[13];
	data[14] = M.data[14];
	data[15] = M.data[15];
	return *this;
}
mat4 &mat4::operator*=(float scalar)
{
	data[0] *= scalar;
	data[1] *= scalar;
	data[2] *= scalar;
	data[3] *= scalar;
	data[4] *= scalar;
	data[5] *= scalar;
	data[6] *= scalar;
	data[7] *= scalar;
	data[8] *= scalar;
	data[9] *= scalar;
	data[10] *= scalar;
	data[11] *= scalar;
	data[12] *= scalar;
	data[13] *= scalar;
	data[14] *= scalar;
	data[15] *= scalar;
	return *this;
}
mat4 &mat4::operator*=(const mat4 &M)
{
	mat4 temp;
	temp.data[0] = M.data[0] * data[0] + M.data[4] * data[1] + M.data[8] * data[2] + M.data[12] * data[3];
	temp.data[1] = M.data[1] * data[0] + M.data[5] * data[1] + M.data[9] * data[2] + M.data[13] * data[3];
	temp.data[2] = M.data[2] * data[0] + M.data[6] * data[1] + M.data[10] * data[2] + M.data[14] * data[3];
	temp.data[3] = M.data[3] * data[0] + M.data[7] * data[1] + M.data[11] * data[2] + M.data[15] * data[3];
	temp.data[4] = M.data[0] * data[4] + M.data[4] * data[5] + M.data[8] * data[6] + M.data[12] * data[7];
	temp.data[5] = M.data[1] * data[4] + M.data[5] * data[5] + M.data[9] * data[6] + M.data[13] * data[7];
	temp.data[6] = M.data[2] * data[4] + M.data[6] * data[5] + M.data[10] * data[6] + M.data[14] * data[7];
	temp.data[7] = M.data[3] * data[4] + M.data[7] * data[5] + M.data[11] * data[6] + M.data[15] * data[7];
	temp.data[8] = M.data[0] * data[8] + M.data[4] * data[9] + M.data[8] * data[10] + M.data[12] * data[11];
	temp.data[9] = M.data[1] * data[8] + M.data[5] * data[9] + M.data[9] * data[10] + M.data[13] * data[11];
	temp.data[10] = M.data[2] * data[8] + M.data[6] * data[9] + M.data[10] * data[10] + M.data[14] * data[11];
	temp.data[11] = M.data[3] * data[8] + M.data[7] * data[9] + M.data[11] * data[10] + M.data[15] * data[11];
	temp.data[12] = M.data[0] * data[12] + M.data[4] * data[13] + M.data[8] * data[14] + M.data[12] * data[15];
	temp.data[13] = M.data[1] * data[12] + M.data[5] * data[13] + M.data[9] * data[14] + M.data[13] * data[15];
	temp.data[14] = M.data[2] * data[12] + M.data[6] * data[13] + M.data[10] * data[14] + M.data[14] * data[15];
	temp.data[15] = M.data[3] * data[12] + M.data[7] * data[13] + M.data[11] * data[14] + M.data[15] * data[15];

	data[0] = temp.data[0];
	data[1] = temp.data[1];
	data[2] = temp.data[2];
	data[3] = temp.data[3];
	data[4] = temp.data[4];
	data[5] = temp.data[5];
	data[6] = temp.data[6];
	data[7] = temp.data[7];
	data[8] = temp.data[8];
	data[9] = temp.data[9];
	data[10] = temp.data[10];
	data[11] = temp.data[11];
	data[12] = temp.data[12];
	data[13] = temp.data[13];
	data[14] = temp.data[14];
	data[15] = temp.data[15];
	return *this;
}
mat4 &mat4::operator/=(float scalar)
{
	data[0] /= scalar;
	data[1] /= scalar;
	data[2] /= scalar;
	data[3] /= scalar;
	data[4] /= scalar;
	data[5] /= scalar;
	data[6] /= scalar;
	data[7] /= scalar;
	data[8] /= scalar;
	data[9] /= scalar;
	data[10] /= scalar;
	data[11] /= scalar;
	data[12] /= scalar;
	data[13] /= scalar;
	data[14] /= scalar;
	data[15] /= scalar;
	return *this;
}
mat4 &mat4::operator+=(const mat4 &M)
{
	data[0] += M.data[0];
	data[1] += M.data[1];
	data[2] += M.data[2];
	data[3] += M.data[3];
	data[4] += M.data[4];
	data[5] += M.data[5];
	data[6] += M.data[6];
	data[7] += M.data[7];
	data[8] += M.data[8];
	data[9] += M.data[9];
	data[10] += M.data[10];
	data[11] += M.data[11];
	data[12] += M.data[12];
	data[13] += M.data[13];
	data[14] += M.data[14];
	data[15] += M.data[15];
	return *this;
}
mat4 &mat4::operator-=(const mat4 &M)
{
	data[0] -= M.data[0];
	data[1] -= M.data[1];
	data[2] -= M.data[2];
	data[3] -= M.data[3];
	data[4] -= M.data[4];
	data[5] -= M.data[5];
	data[6] -= M.data[6];
	data[7] -= M.data[7];
	data[8] -= M.data[8];
	data[9] -= M.data[9];
	data[10] -= M.data[10];
	data[11] -= M.data[11];
	data[12] -= M.data[12];
	data[13] -= M.data[13];
	data[14] -= M.data[14];
	data[15] -= M.data[15];
	return *this;
}
mat4 mat4::operator*(float scalar) const
{
	mat4 temp;
	temp.data[0] *= scalar;
	temp.data[1] *= scalar;
	temp.data[2] *= scalar;
	temp.data[3] *= scalar;
	temp.data[4] *= scalar;
	temp.data[5] *= scalar;
	temp.data[6] *= scalar;
	temp.data[7] *= scalar;
	temp.data[8] *= scalar;
	temp.data[9] *= scalar;
	temp.data[10] *= scalar;
	temp.data[11] *= scalar;
	temp.data[12] *= scalar;
	temp.data[13] *= scalar;
	temp.data[14] *= scalar;
	temp.data[15] *= scalar;
	return temp;
}
vec4 mat4::operator*(const vec4 &M) const
{
	vec4 temp;
	temp.x = M.x * data[0] + M.y * data[1] + M.z * data[2] + M.w * data[3];
	temp.y = M.x * data[4] + M.y * data[5] + M.z * data[6] + M.w * data[7];
	temp.z = M.x * data[8] + M.y * data[9] + M.z * data[10] + M.w * data[11];
	temp.w = M.x * data[12] + M.y * data[13] + M.z * data[14] + M.w * data[15];
	return temp;
}
mat4 mat4::operator*(const mat4 &M) const
{
	mat4 temp;
	temp.data[0] = M.data[0] * data[0] + M.data[4] * data[1] + M.data[8] * data[2] + M.data[12] * data[3];
	temp.data[1] = M.data[1] * data[0] + M.data[5] * data[1] + M.data[9] * data[2] + M.data[13] * data[3];
	temp.data[2] = M.data[2] * data[0] + M.data[6] * data[1] + M.data[10] * data[2] + M.data[14] * data[3];
	temp.data[3] = M.data[3] * data[0] + M.data[7] * data[1] + M.data[11] * data[2] + M.data[15] * data[3];
	temp.data[4] = M.data[0] * data[4] + M.data[4] * data[5] + M.data[8] * data[6] + M.data[12] * data[7];
	temp.data[5] = M.data[1] * data[4] + M.data[5] * data[5] + M.data[9] * data[6] + M.data[13] * data[7];
	temp.data[6] = M.data[2] * data[4] + M.data[6] * data[5] + M.data[10] * data[6] + M.data[14] * data[7];
	temp.data[7] = M.data[3] * data[4] + M.data[7] * data[5] + M.data[11] * data[6] + M.data[15] * data[7];
	temp.data[8] = M.data[0] * data[8] + M.data[4] * data[9] + M.data[8] * data[10] + M.data[12] * data[11];
	temp.data[9] = M.data[1] * data[8] + M.data[5] * data[9] + M.data[9] * data[10] + M.data[13] * data[11];
	temp.data[10] = M.data[2] * data[8] + M.data[6] * data[9] + M.data[10] * data[10] + M.data[14] * data[11];
	temp.data[11] = M.data[3] * data[8] + M.data[7] * data[9] + M.data[11] * data[10] + M.data[15] * data[11];
	temp.data[12] = M.data[0] * data[12] + M.data[4] * data[13] + M.data[8] * data[14] + M.data[12] * data[15];
	temp.data[13] = M.data[1] * data[12] + M.data[5] * data[13] + M.data[9] * data[14] + M.data[13] * data[15];
	temp.data[14] = M.data[2] * data[12] + M.data[6] * data[13] + M.data[10] * data[14] + M.data[14] * data[15];
	temp.data[15] = M.data[3] * data[12] + M.data[7] * data[13] + M.data[11] * data[14] + M.data[15] * data[15];
	return temp;
}
mat4 mat4::operator/(float scalar) const
{
	mat4 temp;
	temp.data[0] = data[0] / scalar;
	temp.data[1] = data[1] / scalar;
	temp.data[2] = data[2] / scalar;
	temp.data[3] = data[3] / scalar;
	temp.data[4] = data[4] / scalar;
	temp.data[5] = data[5] / scalar;
	temp.data[6] = data[6] / scalar;
	temp.data[7] = data[7] / scalar;
	temp.data[8] = data[8] / scalar;
	temp.data[9] = data[9] / scalar;
	temp.data[10] = data[10] / scalar;
	temp.data[11] = data[11] / scalar;
	temp.data[12] = data[12] / scalar;
	temp.data[13] = data[13] / scalar;
	temp.data[14] = data[14] / scalar;
	temp.data[15] = data[15] / scalar;
	return temp;
}
mat4 mat4::operator+(const mat4 &M) const
{
	mat4 temp;
	temp.data[0] = data[0] + M.data[0];
	temp.data[1] = data[1] + M.data[1];
	temp.data[2] = data[2] + M.data[2];
	temp.data[3] = data[3] + M.data[3];
	temp.data[4] = data[4] + M.data[4];
	temp.data[5] = data[5] + M.data[5];
	temp.data[6] = data[6] + M.data[6];
	temp.data[7] = data[7] + M.data[7];
	temp.data[8] = data[8] + M.data[8];
	temp.data[9] = data[9] + M.data[9];
	temp.data[10] = data[10] + M.data[10];
	temp.data[11] = data[11] + M.data[11];
	temp.data[12] = data[12] + M.data[12];
	temp.data[13] = data[13] + M.data[13];
	temp.data[14] = data[14] + M.data[14];
	temp.data[15] = data[15] + M.data[15];
	return temp;
}
mat4 mat4::operator-(const mat4 &M) const
{
	mat4 temp;
	temp.data[0] = data[0] - M.data[0];
	temp.data[1] = data[1] - M.data[1];
	temp.data[2] = data[2] - M.data[2];
	temp.data[3] = data[3] - M.data[3];
	temp.data[4] = data[4] - M.data[4];
	temp.data[5] = data[5] - M.data[5];
	temp.data[6] = data[6] - M.data[6];
	temp.data[7] = data[7] - M.data[7];
	temp.data[8] = data[8] - M.data[8];
	temp.data[9] = data[9] - M.data[9];
	temp.data[10] = data[10] - M.data[10];
	temp.data[11] = data[11] - M.data[11];
	temp.data[12] = data[12] - M.data[12];
	temp.data[13] = data[13] - M.data[13];
	temp.data[14] = data[14] - M.data[14];
	temp.data[15] = data[15] - M.data[15];
	return temp;
}
mat4 mat4::operator-() const
{
	mat4 temp;
	temp.data[0] = -data[0];
	temp.data[1] = -data[1];
	temp.data[2] = -data[2];
	temp.data[3] = -data[3];
	temp.data[4] = -data[4];
	temp.data[5] = -data[5];
	temp.data[6] = -data[6];
	temp.data[7] = -data[7];
	temp.data[8] = -data[8];
	temp.data[9] = -data[9];
	temp.data[10] = -data[10];
	temp.data[11] = -data[11];
	temp.data[12] = -data[12];
	temp.data[13] = -data[13];
	temp.data[14] = -data[14];
	temp.data[15] = -data[15];
	return temp;
}
bool mat4::operator==(const mat4 &M) const
{
	return (data[0] == M.data[0] && data[1] == M.data[1] && data[2] == M.data[2] && data[3] == M.data[3] && data[4] == M.data[4] && data[5] == M.data[5] && data[6] == M.data[6] && data[7] == M.data[7] && data[8] == M.data[8] && data[9] == M.data[9] && data[10] == M.data[10] && data[11] == M.data[11] && data[12] == M.data[12] && data[13] == M.data[13] && data[14] == M.data[14] && data[15] == M.data[15]);
}
void mat4::RotateX(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat4 rotation(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos(degrees), -sin(degrees), 0.0f, 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * (*this);
}
void mat4::RotateY(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat4 rotation(cos(degrees), 0.0f, sin(degrees), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(degrees), 0.0f, cos(degrees), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * (*this);
}
void mat4::RotateZ(float degrees)
{
	degrees *= float(M_PI / 180.0f);
	mat4 rotation(cos(degrees), -sin(degrees), 0.0f, 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * (*this);
}
void mat4::Rotate(float x, float y, float z, float degrees)
{

}
void mat4::Translate(vec3 translation)
{
	*this = mat4(1.0f, 0.0f, 0.0f, translation.x, 0.0f, 1.0f, 0.0f, translation.y, 0.0f, 0.0f, 1.0f, translation.z, 0.0f, 0.0f, 0.0f, 1.0f) * (*this);
}
void mat4::Scale(vec3 scaleFactor)
{
	*this = mat4(scaleFactor.x, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.y, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) * (*this);
}
mat4 mat4::getScale(vec3 scaleFactor)
{
	return mat4(scaleFactor.x, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.y, 0.0f, 0.0f, 0.0f, 0.0f, scaleFactor.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) * (*this);
}
void mat4::Transpose()
{
	mat4 temp = *this;
	data[0]  = temp.data[0];
	data[1]  = temp.data[4];
	data[2]  = temp.data[8];
	data[3]  = temp.data[12];
	data[4]  = temp.data[1];
	data[5]  = temp.data[5];
	data[6]  = temp.data[9];
	data[7]  = temp.data[13];
	data[8]  = temp.data[2];
	data[9]  = temp.data[6];
	data[10] = temp.data[10];
	data[11] = temp.data[14];
	data[12] = temp.data[3];
	data[13] = temp.data[7];
	data[14] = temp.data[11];
	data[15] = temp.data[15];
}
mat4 mat4::getTranspose() const
{
	mat4 temp;
	temp.data[0] = data[0];
	temp.data[1] = data[4];
	temp.data[2] = data[8];
	temp.data[3] = data[12];
	temp.data[4] = data[1];
	temp.data[5] = data[5];
	temp.data[6] = data[9];
	temp.data[7] = data[13];
	temp.data[8] = data[2];
	temp.data[9] = data[6];
	temp.data[10] = data[10];
	temp.data[11] = data[14];
	temp.data[12] = data[3];
	temp.data[13] = data[7];
	temp.data[14] = data[11];
	temp.data[15] = data[15];
	return temp;
}
mat4 mat4::getInverse() const
{
	mat3 tempRot(*this);
	vec3 tempPos = getPos();
	tempRot.Transpose();
	tempPos = -tempRot * tempPos;
	return mat4(tempRot, tempPos);
}
vec3 mat4::getPos() const
{
	return vec3(data[3], data[7], data[11]);
}
void mat4::setPos(vec3 newPos)
{
	data[3] = newPos.x;
	data[7] = newPos.y;
	data[11] = newPos.z;
}
vec3 mat4::getForward()
{
	return vec3(data[3], data[6], data[10]);
}
vec3 mat4::getUp()
{
	return vec3(data[1], data[5], data[9]);
}
void mat4::output()
{
	std::cout << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << "\n";
	std::cout << data[4] << " " << data[5] << " " << data[6] << " " << data[7] << "\n";
	std::cout << data[8] << " " << data[9] << " " << data[10] << " " << data[11] << "\n";
	std::cout << data[12] << " " << data[13] << " " << data[14] << " " << data[15] << "\n";
}
void mat4::Frustum(float fovyDegrees, float aspect, float near, float far)
{
	float h = 1.0f / float(tan(((M_PI / 180.0f)*fovyDegrees) / 2.0f));

	data[0] = h / aspect;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;

	data[4] = 0.0f;
	data[5] = h;
	data[6] = 0.0f;
	data[7] = 0.0f;
	
	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = (far + near) / (near - far);
	data[11] = (2.0f * near*far) / (near - far);
	
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = -1.0f;
	data[15] = 0.0f;
}
void mat4::Ortho(float top, float bottom, float left, float right, float near, float far)
{

}
#pragma endregion