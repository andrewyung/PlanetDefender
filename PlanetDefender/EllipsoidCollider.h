#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

struct EllipsoidCollider : ColliderProperties
{
public:
	EllipsoidCollider()
	{
		type = ELLIPSOID;
	}
	void EllipsoidCollider::setOrientation(glm::vec3 center, glm::vec3 dimensions)
	{
		int focalDistance = glm::sqrt(dimensions.x + dimensions.y + dimensions.z);
		//_foci1 = center.x - glm::sqrt(dimensions.x + dimensions.y + dimensions.z);
	}

	glm::vec3 getDimensions()
	{
		return _dimensions;
	}

private:
	glm::vec3 _center;
	glm::vec3 _dimensions;

	glm::vec3 _foci1;
	glm::vec3 _foci2;
	float _fociDistanceSum;
};