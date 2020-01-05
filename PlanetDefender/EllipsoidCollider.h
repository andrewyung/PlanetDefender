#pragma once

#include <glm/glm.hpp>

#include "ColliderProperties.h"

struct EllipsoidCollider : ColliderProperties
{
	glm::vec3 center;
	glm::vec3 dimensions;
};