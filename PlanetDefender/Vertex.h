#pragma once

class Vertex
{
public:
	Vertex(float x, float y, float z)
		:	x(x), y(y), z(z) {};

	Vertex(	float x, float y, float z, 
			float r, float g, float b, float a,
			float xUV, float yUV, 
			float xNormal, float yNormal, float zNormal)

		:	x(x), y(y), z(z), 
			r(r), g(g), b(b), a(a), 
			xUV(xUV), yUV(yUV),
			xNormal(xNormal), yNormal(yNormal), zNormal(zNormal) {};
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

	//void normalize();
	
	//3d vector
	float x, y, z;
	//color with alpha
	float r, g, b, a;
	//normals
	float xNormal, yNormal, zNormal;
	//texture coordinate
	float xUV, yUV;
};

