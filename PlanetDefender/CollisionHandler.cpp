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
								rayToTriangleCollisionCheck(std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]), 
															std::static_pointer_cast<TriangleCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));
								break;
							case collision_pair(RAY, ELLIPSOID):
								rayToEllipsoidCollisionCheck(std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
															 std::static_pointer_cast<EllipsoidCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));
								break;

						}
					}
				}
			}
		}
	}
}

void rayToTriangleCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<TriangleCollider> col2)
{
}

void rayToEllipsoidCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<EllipsoidCollider> col2)
{

	if (auto properties = std::dynamic_pointer_cast<EllipsoidCollider>(vaoInfo->colliderProp[destColliderIndex]))
	{
		glm::mat4 transformation = vaoInfo->translation * vaoInfo->rotation * vaoInfo->scale * glm::scale(glm::mat4(), properties->getDimensions()); // include dimension!!!!
		glm::mat4 ellipsoidSpace = glm::inverse(transformation);

		// NOT IMPLEMENTED
		// With ellipsoidSpace the ellipsoid is a unit circle at (0, 0, 0)
		//glm::distance()
	}
	else if (auto properties = std::dynamic_pointer_cast<TriangleCollider>(vaoInfo->colliderProp[destColliderIndex]))
	{

	}
}