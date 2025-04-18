#pragma once

#include "Vector.h"

class mat4;

class mat3
{
public:
	float data[9];

	mat3();
	mat3(float Data[9]);
	mat3(mat4 dataMat);
	mat3(float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8);

	//overloaded operators
	mat3 &operator=(const mat3 &M);
	mat3 &operator*=(float scalar);
	mat3 &operator*=(const mat3 &M);
	mat3 &operator/=(float scalar);
	mat3 &operator+=(const mat3 &M);
	mat3 &operator-=(const mat3 &M);
	mat3 operator*(float scalar) const;
	vec2 operator*(const vec2 &M) const;
	vec3 operator*(const vec3 &M) const;
	mat3 operator*(const mat3 &M) const;
	mat3 operator/(float scalar) const;
	mat3 operator+(const mat3 &M) const;
	mat3 operator-(const mat3 &M) const;
	mat3 operator-() const;
	bool operator==(const mat3 &M) const;

	void RotateX(float degrees);
	void RotateY(float degrees);
	mat3 getRotateY(float degrees);
	void RotateZ(float degrees);
	//Axis angle
	void Rotate(float x, float y, float z, float degrees);
	//void Rotate(const vec3 &vec, float degrees);

	void Scale(vec3 scaleFactor);
	mat3 getScale(vec3 scaleFactor);

	void Transpose();
	//void Translate(float x, float y, float z);
	//void Translate(const vec3 &vec);

	vec3 getColumn(int colNum);

	void output();

	void setIdent();

	//Creates a perspective projection
	void Frustum();
	//Creates a orthographic projection
	void Ortho();
};

class mat4
{
public:
	float data[16];

	mat4();
	mat4(float Data[16]);
	mat4(float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, float d9, float d10, float d11, float d12, float d13, float d14, float d15);
	mat4(mat3 rotation, vec3 position);

	//overloaded operators
	mat4 &operator=(const mat4 &M);
	mat4 &operator*=(float scalar);
	mat4 &operator*=(const mat4 &M);
	mat4 &operator/=(float scalar);
	mat4 &operator+=(const mat4 &M);
	mat4 &operator-=(const mat4 &M);
	mat4 operator*(float scalar) const;
	vec4 operator*(const vec4 &M) const;
	mat4 operator*(const mat4 &M) const;
	mat4 operator/(float scalar) const;
	mat4 operator+(const mat4 &M) const;
	mat4 operator-(const mat4 &M) const;
	mat4 operator-() const;
	bool operator==(const mat4 &M) const;

	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);
	//Axis angle
	void Rotate(float x, float y, float z, float degrees);
	//void Rotate(const vec3 &vec, float degrees);

	void Translate(vec3 translation);
	//void Translate(const vec3 &vec);

	void Scale(vec3 scaleFactor);
	mat4 getScale(vec3 scaleFactor);

	void Transpose();
	mat4 getTranspose() const;
	mat4 getInverse() const;
	vec3 getPos() const;
	void setPos(vec3 newPos);
	vec3 getForward();
	vec3 getUp();

	void output();
	
	//Creates a perspective projection
	void Frustum(float fovyDegrees, float aspect, float near, float far);
	//Creates a orthographic projection
	void Ortho(float top, float bottom, float left, float right, float near, float far);
};