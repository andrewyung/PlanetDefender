#pragma once
class Vertex
{
public:
	Vertex(float x, float y, float z) 
		: x(x), y(y), z(z) {};

	Vertex(float x, float y, float z, float r, float g, float b, float a) 
		: x(x), y(y), z(z), r(r), g(g), b(b), a(a) {};

	Vertex(float x, float y, float z, float r, float g, float b, float a, float UVx, float UVy)
		: x(x), y(y), z(z), r(r), g(g), b(b), a(a), UVx(UVx), UVy(UVy){};
	~Vertex();

	
	//3d vector
	float x, y, z;
	//color with alpha
	float r, g, b, a;
	//uv coordinate
	float UVx, UVy;
};

