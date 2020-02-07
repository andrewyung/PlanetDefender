#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "WindowCanvas.h"
#include "CollisionInfo.h"

#include "EllipsoidCollider.h"
#include "TriangleCollider.h"
#include "RayCollider.h"

class CollisionHandler
{
public:
	void CollisionFrame(std::vector<std::shared_ptr<VAOInfo>> vertexArrayIDs);
};

