#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

class TriangleCollider : ColliderProperties
{
public:
	TriangleCollider(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) : p1(p1), p2(2), p3(p3) 
	{
		type = TRIANGLE;
	}

private:
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
};

