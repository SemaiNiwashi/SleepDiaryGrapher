#pragma once
#include <SFML\Graphics.hpp>

class vec2
{
public:
	float x, y;

	vec2();
	vec2(float X, float Y);
	vec2(sf::Vector2u V); //SFML\Grahpics is for this only, otherwise it can be deleted
	
	//overloaded operators
	vec2 &operator=(const vec2 &other);
	vec2 &operator*=(float scalar);
	vec2 &operator*=(const vec2 &other);
	vec2 &operator/=(float scalar);
	vec2 &operator+=(const float &other);
	vec2 &operator+=(const vec2 &other);
	vec2 &operator-=(const vec2 &other);
	vec2 operator*(float scalar) const;
	vec2 operator*(const vec2 &other) const;
	vec2 operator/(float scalar) const;
	vec2 operator/(const vec2 &other) const;
	vec2 operator+(const float &other) const;
	vec2 operator+(const vec2 &other) const;
	vec2 operator-(const vec2 &other) const;
	vec2 operator-() const;
	bool operator==(const vec2 &other) const;
	bool operator!=(const vec2 &other) const;

	void Rotate(float degrees);
	vec2 Rotated(float degrees);

	//Normalization
	void Normalize();
	vec2 Normalized();
	vec2 Direction() const;
	//Length
	float Length() const;
	float LengthSquared() const;
	//Dot
	float Dot(const vec2 &other) const;

	void output(bool newLine) const;

	//Static functions
	static vec2 Unit() { return vec2(1.0f, 1.0f); }
	static vec2 Zero() { return vec2(0.0f, 0.0f); }
	static vec2 Up() { return vec2(0.0f, 1.0f); }
	static vec2 Right() { return vec2(1.0f, 0.0f); }
};

class vec3
{
public:
	float x, y, z;

	vec3();
	vec3(float X, float Y, float Z);
	vec3(vec2 XY, float Z);

	//overloaded operators
	vec3 &operator=(const vec3 &other);
	vec3 &operator*=(float scalar);
	vec3 &operator*=(vec3 &other);
	vec3 &operator/=(float scalar);
	vec3 &operator+=(const float &other);
	vec3 &operator+=(const vec3 &other);
	vec3 &operator-=(const vec3 &other);
	vec3 operator*(float scalar) const;
	vec3 operator*(const vec3 &other) const;
	vec3 operator/(float scalar) const;
	vec3 operator+(const float &other) const;
	vec3 operator+(const vec3 &other) const;
	vec3 operator-(const float &other) const;
	vec3 operator-(const vec3 &other) const;
	vec3 operator-() const;
	bool operator==(const vec3 &other) const;
	bool operator!=(const vec3 &other) const;
	bool operator>=(const vec3 &other) const;
	bool operator<=(const vec3 &other) const;

	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);
	void Rotate(const vec3 &rot);
	void RotateAxis(float degrees, vec3 axis);
	void setXY(const vec2 &other);
	void setXY(const vec3 &other);
	void SetAnglesFromVector();
	void SetAnglesFromOtherVector(vec3 &other);
	vec3 AnglesFromVector();


	//Normalization
	void Normalize();
	vec3 Normalized();
	vec3 Direction() const;
	//Length
	float Length() const;
	float LengthSquared() const;
	//Dot
	float Dot(const vec3 &other) const;
	vec3 Cross(const vec3& other) const;

	void output(bool newLine) const;

	//Static functions
	static vec3 Unit() { return vec3(1.0f, 1.0f, 1.0f); }
	static vec3 Zero() { return vec3(0.0f, 0.0f, 0.0f); }
	static vec3 Up() { return vec3(0.0f, 1.0f, 0.0f); }
	static vec3 Right() { return vec3(1.0f, 0.0f, 0.0f); }
	static vec3 Forward() { return vec3(0.0f, 0.0f, 1.0f); }
	static vec3 Down() { return vec3(0.0f, -1.0f, 0.0f); }
	static vec3 Left() { return vec3(-1.0f, 0.0f, 0.0f); }
	static vec3 Back() { return vec3(0.0f, 0.0f, -1.0f); }
};

class vec3i
{
public:
	int x, y, z;

	vec3i();
	vec3i(int X, int Y, int Z);

	//overloaded operators
	vec3i &operator=(const vec3i &other);
	vec3i &operator*=(int scalar);
	vec3i &operator/=(int scalar);
	vec3i &operator+=(const vec3i &other);
	vec3i &operator-=(const vec3i &other);
	vec3i operator*(int scalar) const;
	vec3i operator/(int scalar) const;
	vec3i operator+(const vec3i &other) const;
	vec3i operator-(const vec3i &other) const;
	vec3i operator-() const;
	bool operator==(const vec3i &other) const;

	void output(bool newLine) const;
};

class vec4
{
public:
	float x, y, z, w;

	vec4();
	vec4(float X, float Y, float Z, float W);

	//overloaded operators
	vec4 &operator=(const vec4 &other);
	vec4 &operator*=(float scalar);
	vec4 &operator/=(float scalar);
	vec4 &operator+=(const vec4 &other);
	vec4 &operator-=(const vec4 &other);
	vec4 operator*(float scalar) const;
	vec4 operator*(const vec4 &other) const;
	vec4 operator/(float scalar) const;
	vec4 operator+(const vec4 &other) const;
	vec4 operator-(const vec4 &other) const;
	vec4 operator-() const;
	bool operator==(const vec4 &other) const;
	bool operator!=(const vec4 &other) const;

	//Normalization
	void Normalize();
	vec4 Direction() const;
	//Length
	float Length() const;
	float LengthSquared() const;
	//Dot
	float Dot(const vec4 &other) const;

	void output(bool newLine) const;

	//Static functions
	static vec4 Unit() { return vec4(1.0f, 1.0f, 1.0f, 1.0f); }
	static vec4 Zero() { return vec4(0.0f, 0.0f, 0.0f, 0.0f); }
	static vec4 Up() { return vec4(0.0f, 1.0f, 0.0f, 0.0f); }
	static vec4 Right() { return vec4(1.0f, 0.0f, 0.0f, 0.0f); }
	static vec4 Forward() { return vec4(0.0f, 0.0f, 1.0f, 0.0f); }
	static vec4 Down() { return vec4(0.0f, -1.0f, 0.0f, 0.0f); }
	static vec4 Left() { return vec4(-1.0f, 0.0f, 0.0f, 0.0f); }
	static vec4 Back() { return vec4(0.0f, 0.0f, -1.0f, 0.0f); }
};

class quat :public vec4
{
public:
	quat();
	quat(float X, float Y, float Z, float W);

	void rotByAngleAxis(vec3 n, float theta);
};