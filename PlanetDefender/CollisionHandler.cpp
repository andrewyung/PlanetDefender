#include "CollisionHandler.h"

std::optional<CollisionInfo> rayToTriangleCollisionCheck(glm::mat4 rayMVP, std::shared_ptr<RayCollider> col1, glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col2);
std::optional<CollisionInfo> rayToEllipsoidCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<EllipsoidCollider> col2);

constexpr unsigned int collision_pair(ColliderType t1, ColliderType t2) {
	return (t1 << 16) + t2;
}

void CollisionHandler::CollisionFrame(const std::vector<std::shared_ptr<VAOInfo>> vertexArrayIDs)
{
	// For each VAO
	for (int sourceVAOIndex{ 0 }; sourceVAOIndex < vertexArrayIDs.size(); sourceVAOIndex++)
	{
		std::shared_ptr<VAOInfo> vaoInfo = vertexArrayIDs[sourceVAOIndex];

		// Has velocity and collider
		// Update based on velocity
		vaoInfo->translation = glm::translate(vaoInfo->translation, vaoInfo->velocity * WindowCanvas::deltaCallbackTime);

		if (vaoInfo->drawing)
		{
			//std::cout << vaoInfo->colliderProp.size() << std::endl;
			for (int sourceColliderIndex{ 0 }; sourceColliderIndex < vaoInfo->colliderProp.size(); sourceColliderIndex++)
			{
				for (int destVAOIndex{ 0 }; destVAOIndex < vertexArrayIDs.size(); destVAOIndex++)
				{
					if (destVAOIndex == sourceVAOIndex) continue;

					std::shared_ptr<VAOInfo> dest_vaoInfo = vertexArrayIDs[destVAOIndex];
					// For each collider property
					for (int destColliderIndex{ 0 }; destColliderIndex < dest_vaoInfo->colliderProp.size(); destColliderIndex++)
					{
						//std::cout << vaoInfo->colliderProp[sourceColliderIndex]->type << " : " << dest_vaoInfo->colliderProp[destColliderIndex]->type << std::endl;
						std::optional<CollisionInfo> colInfo{};
						switch (collision_pair(vaoInfo->colliderProp[sourceColliderIndex]->type, dest_vaoInfo->colliderProp[destColliderIndex]->type))
						{
						case collision_pair(RAY, TRIANGLE):
							colInfo = rayToTriangleCollisionCheck(vaoInfo->translation * vaoInfo->rotation * vaoInfo->scale,
								std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
								dest_vaoInfo->translation * dest_vaoInfo->rotation * dest_vaoInfo->scale,
								std::static_pointer_cast<TriangleCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));

							break;
						case collision_pair(RAY, ELLIPSOID):
							colInfo = rayToEllipsoidCollisionCheck(std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
								std::static_pointer_cast<EllipsoidCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));

							break;

						}

						if (colInfo.has_value())
						{
							std::cout << "collide " << colInfo->getCollisionPoint().x << " : " << colInfo->getCollisionPoint().y << " : " << colInfo->getCollisionPoint().z << std::endl;
						}
					}
				}
			}
		}
	}
}

std::optional<CollisionInfo> rayToTriangleCollisionCheck(glm::mat4 rayMVP, std::shared_ptr<RayCollider> col1, glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col2)
{
	const double ep = 0.000001;
	glm::vec3 p0 = triangleMVP * glm::vec4(col2->getP0(), 1.0f);
	glm::vec3 p1 = triangleMVP * glm::vec4(col2->getP1(), 1.0f);
	glm::vec3 p2 = triangleMVP * glm::vec4(col2->getP2(), 1.0f);
	glm::vec3 v2v0 = p2 - p0;
	glm::vec3 v1v0 = p1 - p0;
	glm::vec3 rayv0 = col1->getPoint() - p0;
	glm::vec3 pvec = glm::cross(col1->getDirection(), v2v0);

	float det = glm::dot(v1v0, pvec);
	float invDet = 1.0f / det;

	float u = glm::dot(rayv0, pvec) * invDet;
	
	if (u < 0 || u > 1)
	{
		return std::nullopt;
	}

	glm::vec3 qvec = glm::cross(rayv0, v1v0);

	float v = glm::dot(col1->getDirection(), qvec) * invDet;

	if (v < 0 || u + v > 1)
	{
		return std::nullopt;
	}

	float t = glm::dot(v2v0, qvec) * invDet;

	return CollisionInfo(col1->getPoint() + (col1->getDirection() * t), RAY, TRIANGLE);
}

std::optional<CollisionInfo> rayToEllipsoidCollisionCheck(std::shared_ptr<RayCollider> col1, std::shared_ptr<EllipsoidCollider> col2)
{
	return std::nullopt;
}