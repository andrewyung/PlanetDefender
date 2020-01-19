#include "EllipsoidCollider.h"

// This collider is in local space of the object
void EllipsoidCollider::setOrientation(glm::vec3 center, glm::vec3 dimensions)
{
	int focalDistance = glm::sqrt(dimensions.x + dimensions.y + dimensions.z);
	//_foci1 = center.x - glm::sqrt(dimensions.x + dimensions.y + dimensions.z);
}
