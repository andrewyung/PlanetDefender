#pragma once

#include <iostream>
#include <glm/gtx/string_cast.hpp>

class Vertex
{
private:
	bool _tangentSolved = false;
public:
	//3d vector
	float x, y, z;
	//color with alpha
	float r, g, b, a;
	//normals
	float xNormal, yNormal, zNormal;
	//texture coordinate
	float xUV, yUV;

	float xTangent, yTangent, zTangent;

	Vertex(float x, float y, float z)
		:	x(x), y(y), z(z) {};

	Vertex(	float x, float y, float z, 
			float r, float g, float b, float a,
			float xUV, float yUV, 
			float xNormal, float yNormal, float zNormal)

		:	x(x), y(y), z(z), 
			r(r), g(g), b(b), a(a), 
			xUV(xUV), yUV(yUV),
			xNormal(xNormal), yNormal(yNormal), zNormal(zNormal),
			xTangent(0), yTangent(0), zTangent(0) {};
	//~Vertex();

	void setNormals(float xNormal, float yNormal, float zNormal)
	{
		this->xNormal = xNormal; 
		this->yNormal = yNormal;
		this->zNormal = zNormal;
	}
	void setUVCoordinate(float xUV, float yUV)
	{
		this->xUV = xUV;
		this->yUV = yUV;
	}
	void setColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void calculateTangent(glm::vec3 vector)
	{
		glm::vec3 normal = glm::vec3(xNormal, yNormal, zNormal);
		glm::vec3 tangent = glm::cross(normal, glm::normalize(vector));

		xTangent = tangent.x;
		yTangent = tangent.y;
		zTangent = tangent.z;

		_tangentSolved = true;
	}

	bool tangentSolved()
	{
		return _tangentSolved;
	}

	glm::vec3 getPoint()
	{
		return glm::vec3(x, y, z);
	}
};

