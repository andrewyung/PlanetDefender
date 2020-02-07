#include "CollisionHandler.h"

constexpr unsigned int collision_pair(ColliderType t1, ColliderType t2) {
	return (t1 << 16) + t2;
}

void CollisionHandler::CollisionFrame(std::vector<std::shared_ptr<VAOInfo>> vertexArrayIDs)
{
	// For each VAO
	for (int i{ 0 }; i < vertexArrayIDs.size(); i++)
	{
		std::shared_ptr<VAOInfo> vaoInfo = vertexArrayIDs[i];

		// Has velocity and collider
		// Update based on velocity
		vaoInfo->translation = glm::translate(vaoInfo->translation, vaoInfo->velocity * WindowCanvas::deltaCallbackTime);

		if (vaoInfo->velocity.length > 0 && vaoInfo->colliderProp.size() > 0)
		{
			for (int sourceColliderIndex{ 0 }; sourceColliderIndex < vertexArrayIDs.size(); sourceColliderIndex++)
			{
				for (int destVAOIndex{ 0 }; i < vertexArrayIDs.size(); i++)
				{
					std::shared_ptr<VAOInfo> dest_vaoInfo = vertexArrayIDs[destVAOIndex];
					// For each collider property
					for (int destColliderIndex{ 0 }; destColliderIndex < dest_vaoInfo->colliderProp.size(); destColliderIndex++)
					{
						if (destColliderIndex == sourceColliderIndex) continue;

						switch (collision_pair(vaoInfo->colliderProp[sourceColliderIndex]->type, dest_vaoInfo->colliderProp[destColliderIndex]->type))
						{
							case collision_pair(RAY, TRIANGLE):
								CollisionInfo colInfo = rayToTriangleCollisionCheck(std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]), 
															std::static_pointer_cast<TriangleCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));
								
								break;
							case collision_pair(RAY, ELLIPSOID):
								CollisionInfo colInfo = rayToEllipsoidCollisionCheck(std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
															 std::static_pointer_cast<EllipsoidCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));
								
								break;

						}
					}
				}
			}
		}
	}
}

CollisionInfo rayToTriangleCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<TriangleCollider> col2)
std::optional<CollisionInfo> rayToTriangleCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<TriangleCollider> col2)
{
	glm::vec3 p0 = col2->getP0();
	glm::vec3 p1 = col2->getP1();
	glm::vec3 p2 = col2->getP2();
	glm::vec3 v2v0 = p2 - p0;
	glm::vec3 v1v0 = p1 - p0;
	glm::vec3 rayv0 = col1->getDirection() - p0;

	float a = glm::dot(v1v0, v2v0);
	float b = glm::dot(rayv0, v2v0);
	float c = glm::dot(rayv0, v1v0);
	float d = glm::dot(v2v0, v2v0);
	float e = glm::dot(v1v0, v1v0);

	float s = ((a * b) - (d * c)) / ((a * a) - (d * e));
	if (s < 0)
	{
		return std::nullopt;
	}

	float t = ((a * c) - (e * b)) / ((a * a) - (d * e));

	if (t < 0 || s + t > 1)
	{
		return std::nullopt;
	}

	return CollisionInfo(p0 + (s * v1v0) + (t * v2v0));
}

CollisionInfo rayToEllipsoidCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<EllipsoidCollider> col2)
{
	return CollisionInfo();
}