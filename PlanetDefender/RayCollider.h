#pragma once
#include <glm/glm.hpp>

#include "ColliderProperties.h"

class RayCollider : ColliderProperties
{
public:
	RayCollider(glm::vec3 point, glm::vec3 direction) : point(point), direction(direction)
	{
		type = RAY;
	}

	glm::vec3 getDirection()
	{
		return direction;
	}

	glm::vec3 getPoint()
	{
		return point;
	}

private:
	glm::vec3 point;
	glm::vec3 direction;
};

