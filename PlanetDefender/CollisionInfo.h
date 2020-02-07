#pragma once

#include <glm/glm.hpp>

class CollisionInfo
{
public:
	CollisionInfo(glm::vec3 collisionPoint) : collisionPoint(collisionPoint) {};

private:
	glm::vec3 collisionPoint;
};