#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

class TriangleCollider : ColliderProperties
{
public:
	TriangleCollider(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) : p0(p0), p1(p1), p2(p2) 
	{
		type = TRIANGLE;
	}

	glm::vec3 getP0()
	{
		return p0;
	}
	glm::vec3 getP1()
	{
		return p1;
	}
	glm::vec3 getP2()
	{
		return p2;
	}

private:
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
};

