#pragma once

#include <glm/glm.hpp>

class CollisionInfo
{
public:
	CollisionInfo(glm::vec3 collisionPoint, ColliderType source, ColliderType dest) : collisionPoint(collisionPoint), sourceType(source), destType(dest) {};

	glm::vec3 getCollisionPoint()
	{
		return collisionPoint;
	}

	ColliderType getSourceType()
	{
		return sourceType;
	}

	ColliderType getDestType()
	{
		return destType;
	}

private:
	glm::vec3 collisionPoint;

	ColliderType sourceType;
	ColliderType destType;
};