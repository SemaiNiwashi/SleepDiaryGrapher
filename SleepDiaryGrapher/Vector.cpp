#include "Vector.h"
#include <math.h>
#include <iostream>
#include "Matrix.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//namespace kcc
//{
#pragma region //vec2
vec2::vec2()
{
	x = 0.0f;
	y = 0.0f;
}
vec2::vec2(float X, float Y)
{
	x = X;
	y = Y;
}
vec2::vec2(sf::Vector2u V)
{
	x = V.x;
	y = V.y;
}
vec2 &vec2::operator=(const vec2 &other)
{
	x = other.x;
	y = other.y;
	return *this;
}
vec2 &vec2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}
vec2 &vec2::operator*=(const vec2 &other)
{
	x *= other.x;
	y *= other.x;
	return *this;
}
vec2 &vec2::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}
vec2 &vec2::operator+=(const float &other)
{
	x += other;
	y += other;
	return *this;
}
vec2 &vec2::operator+=(const vec2 &other)
{
	x += other.x;
	y += other.y;
	return *this;
}
vec2 &vec2::operator-=(const vec2 &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
vec2 vec2::operator*(float scalar) const
{
	return vec2(x * scalar, y * scalar);
}
vec2 vec2::operator*(const vec2 &other) const
{
	return vec2(x * other.x, y * other.y);
}
vec2 vec2::operator/(float scalar) const
{
	return vec2(x / scalar, y / scalar);
}
vec2 vec2::operator/(const vec2 &other) const
{
	return vec2(x / other.x, y / other.y);
}
vec2 vec2::operator+(const float &other) const
{
	return vec2(x + other, y + other);
}
vec2 vec2::operator+(const vec2 &other) const
{
	return vec2(x + other.x, y + other.y);
}
vec2 vec2::operator-(const vec2 &other) const
{
	return vec2(x - other.x, y - other.y);
}
vec2 vec2::operator-() const
{
	return vec2(-x, -y);
}
bool vec2::operator==(const vec2 &other) const
{
	return (x == other.x && y == other.y);
}
bool vec2::operator!=(const vec2 &other) const
{
	return (x != other.x || y != other.y);
}
void vec2::Rotate(float degrees)
{
	degrees *= float(M_PI / 180);
	mat3 rotation(cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * *this;
}
vec2 vec2::Rotated(float degrees)
{
	degrees *= float(M_PI / 180);
	mat3 rotation(cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 1.0f);
	return rotation * *this;
}
void vec2::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
}
vec2 vec2::Normalized()
{
	float length = Length();
	return vec2(x / length, y / length);
}
vec2 vec2::Direction() const
{
	vec2 temp(*this);
	temp.Normalize();
	return temp;
}
float vec2::Length() const
{
	return sqrtf(x * x + y * y);
}
float vec2::LengthSquared() const
{
	return x * x + y * y;
}
float vec2::Dot(const vec2 &other) const
{
	return x * other.x + y * other.y;
}
void vec2::output(bool newLine) const
{
	std::cout << "(" << x << ", " << y << ")";
	if (newLine)
	{
		std::cout << std::endl;
	}
}
#pragma endregion

#pragma region //vec3
vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
vec3::vec3(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}
vec3::vec3(vec2 XY, float Z)
{
	x = XY.x;
	y = XY.y;
	z = Z;
}
vec3 &vec3::operator=(const vec3 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}
vec3 &vec3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
vec3 &vec3::operator*=(vec3 &other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}
vec3 &vec3::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}
vec3 &vec3::operator+=(const float &other)
{
	x += other;
	y += other;
	z += other;
	return *this;
}
vec3 &vec3::operator+=(const vec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
vec3 &vec3::operator-=(const vec3 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
vec3 vec3::operator*(float scalar) const
{
	return vec3(x * scalar, y * scalar, z * scalar);
}
vec3 vec3::operator*(const vec3 &other) const
{
	return vec3(x * other.x, y * other.y, z * other.z);
}
vec3 vec3::operator/(float scalar) const
{
	return vec3(x / scalar, y / scalar, z / scalar);
}
vec3 vec3::operator+(const float &other) const
{
	return vec3(x + other, y + other, z + other);
}
vec3 vec3::operator+(const vec3 &other) const
{
	return vec3(x + other.x, y + other.y, z + other.z);
}
vec3 vec3::operator-(const float &other) const
{
	return vec3(x - other, y - other, z - other);
}
vec3 vec3::operator-(const vec3 &other) const
{
	return vec3(x - other.x, y - other.y, z - other.z);
}
vec3 vec3::operator-() const
{
	return vec3(-x, -y, -z);
}
bool vec3::operator==(const vec3 &other) const
{
	return (x == other.x && y == other.y && z == other.z);
}
bool vec3::operator!=(const vec3 &other) const
{
	return (x != other.x || y != other.y || z != other.z);
}
bool vec3::operator>=(const vec3 &other) const
{
	return (x >= other.x && y >= other.y && z >= other.z);
}
bool vec3::operator<=(const vec3 &other) const
{
	return (x <= other.x && y <= other.y && z <= other.z);
}
void vec3::RotateX(float degrees)
{
	degrees *= float(M_PI / 180);
	mat3 rotation(1.0f, 0.0f, 0.0f, 0.0f, cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees));
	*this = rotation * *this;
}
void vec3::RotateY(float degrees)
{
	degrees *= float(M_PI / 180);
	mat3 rotation(cos(degrees), 0.0f, sin(degrees), 0.0f, 1.0f, 0.0f, -sin(degrees), 0.0f, cos(degrees));
	*this = rotation * *this;
}
void vec3::RotateZ(float degrees)
{
	degrees *= float(M_PI / 180);
	mat3 rotation(cos(degrees), -sin(degrees), 0.0f, sin(degrees), cos(degrees), 0.0f, 0.0f, 0.0f, 1.0f);
	*this = rotation * *this;
}
void vec3::Rotate(const vec3 &rot)
{
	RotateZ(rot.z);
	RotateY(rot.y);
	RotateX(rot.x);
}
void vec3::RotateAxis(float degrees, vec3 axis)
{
	RotateZ(degrees*axis.z);
	RotateY(degrees*axis.y);
	RotateX(degrees*axis.x);
}
void vec3::setXY(const vec2 &other)
{
	x = other.x;
	y = other.y;
}
void vec3::setXY(const vec3 &other)
{
	x = other.x;
	y = other.y;
}
void vec3::SetAnglesFromVector()
{
	vec3 original(x, y, z);
	y = 90 + atan(original.z / original.x) * float(180 / M_PI);
	if (original.x < 0)
	{
		y -= 180;
		y = 360 + y;
	}
	x = atan(original.y / (sqrtf(original.x * original.x + original.z * original.z))) * float(180 / M_PI) * -1;
	z = 0;
}
void vec3::SetAnglesFromOtherVector(vec3 &other)
{
	y = 90 + atan(other.z / other.x) * float(180 / M_PI);
	if (other.x < 0)
	{
		y -= 180;
		y = 360 + y;
	}
	x = atan(other.y / (sqrtf(other.x * other.x + other.z * other.z))) * float(180 / M_PI) * -1;
	z = 0;
}
vec3 vec3::AnglesFromVector()
{
	vec3 temp;
	temp.y = 90 + atan(z / x) * float(180 / M_PI);
	if (x < 0)
	{
		temp.y -= 180;
		temp.y = 360 + temp.y;
	}
	temp.x = atan(y / (sqrtf(x * x + z * z))) * float(180 / M_PI) * -1;
	temp.z = 0;
	return temp;
}
void vec3::Normalize()
{
	float length = Length();
	if (length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}
vec3 vec3::Normalized()
{
	float length = Length();
	return vec3(x / length, y / length, z / length);
}
vec3 vec3::Direction() const
{
	vec3 temp(*this);
	temp.Normalize();
	return temp;
}
float vec3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}
float vec3::LengthSquared() const
{
	return x * x + y * y + z * z;
}
float vec3::Dot(const vec3 &other) const
{
	return x * other.x + y * other.y + z * other.z;
}
vec3 vec3::Cross(const vec3& other) const
{
	return vec3(y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}
void vec3::output(bool newLine) const
{
	std::cout << "(" << x << ", " << y << ", " << z << ")";
	if (newLine)
	{
		std::cout << std::endl;
	}
}
#pragma endregion

#pragma region //vec3i
vec3i::vec3i()
{
	x = 0;
	y = 0;
	z = 0;
}
vec3i::vec3i(int X, int Y, int Z)
{
	x = X;
	y = Y;
	z = Z;
}
vec3i &vec3i::operator=(const vec3i &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}
vec3i &vec3i::operator*=(int scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
vec3i &vec3i::operator/=(int scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}
vec3i &vec3i::operator+=(const vec3i &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
vec3i &vec3i::operator-=(const vec3i &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
vec3i vec3i::operator*(int scalar) const
{
	return vec3i(x * scalar, y * scalar, z * scalar);
}
vec3i vec3i::operator/(int scalar) const
{
	return vec3i(x / scalar, y / scalar, z / scalar);
}
vec3i vec3i::operator+(const vec3i &other) const
{
	return vec3i(x + other.x, y + other.y, z + other.z);
}
vec3i vec3i::operator-(const vec3i &other) const
{
	return vec3i(x - other.x, y - other.y, z - other.z);
}
vec3i vec3i::operator-() const
{
	return vec3i(-x, -y, -z);
}
bool vec3i::operator==(const vec3i &other) const
{
	return (x == other.x && y == other.y && z == other.z);
}
void vec3i::output(bool newLine) const
{
	std::cout << "(" << x << ", " << y << ", " << z << ")";
	if (newLine)
	{
		std::cout << std::endl;
	}
}
#pragma endregion

#pragma region //vec4
vec4::vec4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}
vec4::vec4(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}
vec4 &vec4::operator=(const vec4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}
vec4 &vec4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}
vec4 &vec4::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}
vec4 &vec4::operator+=(const vec4 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}
vec4 &vec4::operator-=(const vec4 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}
vec4 vec4::operator*(float scalar) const
{
	return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}
vec4 vec4::operator*(const vec4 &other) const
{
	return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
}
vec4 vec4::operator/(float scalar) const
{
	return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}
vec4 vec4::operator+(const vec4 &other) const
{
	return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}
vec4 vec4::operator-(const vec4 &other) const
{
	return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}
vec4 vec4::operator-() const
{
	return vec4(-x, -y, -z, -w);
}
bool vec4::operator==(const vec4 &other) const
{
	return (x == other.x && y == other.y && z == other.z && w == other.w);
}
bool vec4::operator!=(const vec4 &other) const
{
	return (x != other.x || y != other.y || z != other.z || w != other.w);
}
void vec4::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}
vec4 vec4::Direction() const
{
	vec4 temp(*this);
	temp.Normalize();
	return temp;
}
float vec4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}
float vec4::LengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}
float vec4::Dot(const vec4 &other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}
void vec4::output(bool newLine) const
{
	std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	if (newLine)
	{
		std::cout << std::endl;
	}
}
#pragma endregion

#pragma region //quat
quat::quat()
	: vec4()
{
}
quat::quat(float X, float Y, float Z, float W)
	: vec4(X, Y, Z, W)
{
}
void quat::rotByAngleAxis(vec3 n, float theta)
{
	w = cos(theta / 2);
	x = n.x * sin(theta / 2);
	y = n.y * sin(theta / 2);
	z = n.z * sin(theta / 2);
}
#pragma endregion
//}