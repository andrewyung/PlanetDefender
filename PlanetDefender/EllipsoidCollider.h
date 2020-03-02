#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

struct EllipsoidCollider : public ColliderProperties
{
public:
	EllipsoidCollider(glm::vec3 center, glm::vec3 dimensions) : center(center), dimensions(dimensions)
	{
		type = ELLIPSOID;
	}
	void EllipsoidCollider::setCenter(glm::vec3 center)
	{
		this->center = center;
	}
	void EllipsoidCollider::setDimension(glm::vec3 dimension)
	{
		this->dimensions = dimensions;
	}

	glm::vec3 getDimensions()
	{
		return dimensions;
	}
	glm::vec3 getCenter()
	{
		return center;
	}

private:
	glm::vec3 center;
	glm::vec3 dimensions;
};