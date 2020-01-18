#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

struct EllipsoidCollider : ColliderProperties
{
public:
	void setOrientation(glm::vec3 center, glm::vec3 dimensions);

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