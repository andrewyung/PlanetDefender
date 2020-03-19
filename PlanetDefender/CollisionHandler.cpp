#include "CollisionHandler.h"

std::optional<CollisionInfo> rayToTriangleCollisionCheck(std::shared_ptr<RayCollider> col1, glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col2);
std::optional<CollisionInfo> triangleToSphereCollisionCheck(glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col1, glm::mat4 ellipsoidMVP, std::shared_ptr<EllipsoidCollider> col2);

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
		vaoInfo->translation = glm::translate(vaoInfo->translation, glm::mat3(vaoInfo->rotation) * vaoInfo->velocity * WindowCanvas::deltaCallbackTime);

		if (!vaoInfo->drawing) continue;
		//std::cout << vaoInfo->colliderProp.size() << std::endl;
		for (int sourceColliderIndex{ 0 }; sourceColliderIndex < vaoInfo->colliderProp.size(); sourceColliderIndex++)
		{
			for (int destVAOIndex{ 0 }; destVAOIndex < vertexArrayIDs.size(); destVAOIndex++)
			{
				if (destVAOIndex == sourceVAOIndex) continue;

				std::shared_ptr<VAOInfo> dest_vaoInfo = vertexArrayIDs[destVAOIndex];

				if (!dest_vaoInfo->drawing) continue;

				// For each collider property
				for (int destColliderIndex{ 0 }; destColliderIndex < dest_vaoInfo->colliderProp.size(); destColliderIndex++)
				{
					//std::cout << vaoInfo->colliderProp[sourceColliderIndex]->type << " : " << dest_vaoInfo->colliderProp[destColliderIndex]->type << std::endl;
					std::optional<CollisionInfo> colInfo{};
					switch (collision_pair(vaoInfo->colliderProp[sourceColliderIndex]->type, dest_vaoInfo->colliderProp[destColliderIndex]->type))
					{
					case collision_pair(RAY, TRIANGLE):
						colInfo = rayToTriangleCollisionCheck(	std::static_pointer_cast<RayCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
																dest_vaoInfo->translation * dest_vaoInfo->rotation * dest_vaoInfo->scale,
																std::static_pointer_cast<TriangleCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));

						break;
					case collision_pair(TRIANGLE, ELLIPSOID):
						colInfo = triangleToSphereCollisionCheck(	vaoInfo->translation * vaoInfo->rotation * vaoInfo->scale,
																	std::static_pointer_cast<TriangleCollider>(vaoInfo->colliderProp[sourceColliderIndex]),
																	dest_vaoInfo->translation * dest_vaoInfo->rotation * dest_vaoInfo->scale,
																	std::static_pointer_cast<EllipsoidCollider>(dest_vaoInfo->colliderProp[destColliderIndex]));

break;

					}

					if (colInfo.has_value())
					{
						if (dest_vaoInfo->onCollisionCallback) dest_vaoInfo->onCollisionCallback(*dest_vaoInfo, *vaoInfo, colInfo.value());
						//std::cout << "collide " << colInfo->getCollisionPoint().x << " : " << colInfo->getCollisionPoint().y << " : " << colInfo->getCollisionPoint().z << std::endl;
					}
				}
			}
		}
	}
}

std::optional<CollisionInfo> rayToTriangleCollisionCheck(std::shared_ptr<RayCollider> col1, glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col2)
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

// This only checks for circles that lay inside of triangle (no vertex or edge detection with circle).
std::optional<CollisionInfo> triangleToSphereCollisionCheck(glm::mat4 triangleMVP, std::shared_ptr<TriangleCollider> col1, glm::mat4 ellipsoidMVP, std::shared_ptr<EllipsoidCollider> col2)
{
	float epsilon = 0.0000005f;

	// Construct matrix for ellipsoid space
	glm::mat4 ellipsoidInvMatrix = inverse(glm::scale(glm::mat4(1.0f), col2->getDimensions()) * glm::translate(glm::mat4(1.0f), col2->getCenter()));

	glm::vec3 v0 = ellipsoidInvMatrix * triangleMVP * glm::vec4{ col1->getP0(), 1 };
	glm::vec3 v1 = ellipsoidInvMatrix * triangleMVP * glm::vec4{ col1->getP1(), 1 };
	glm::vec3 v2 = ellipsoidInvMatrix * triangleMVP * glm::vec4{ col1->getP2(), 1 };

	glm::vec3 triangleNorm = normalize(glm::cross(v2 - v1, v0 - v1));

	// project onto the normal
	glm::vec3 nearestPlanePointToCenter = triangleNorm * (glm::dot(triangleNorm, -v0));

	// No collision from plane (on same plane as triangle) to sphere
	if (length(nearestPlanePointToCenter) > 1)
	{
		//std::cout << "fail chk" << distanceToCenter << std::endl;
		return std::nullopt;
	}

	// This point lays onto the plane of the triangle
	glm::vec3 intersectionPointonPlane = -nearestPlanePointToCenter;

	/*
	// Check if point is within triangle with barycentric coordinates
	//https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	glm::vec3	v1v0 = v1 - v0,
				v2v0 = v2 - v0,
				pv0 = intersectionPointonPlane - v0;
	float d00 = glm::dot(v1v0, v1v0);
	float d01 = glm::dot(v1v0, v2v0);
	float d11 = glm::dot(v2v0, v2v0);
	float d20 = glm::dot(pv0, v1v0);
	float d21 = glm::dot(pv0, v2v0);
	float denom = d00 * d11 - d01 * d01;
	float gamma = (d11 * d20 - d01 * d21) / denom;
	float beta = (d00 * d21 - d01 * d20) / denom;
	float alpha = 1.0f - gamma - beta;

	float val = gamma + beta + alpha;
	if (gamma > 1 || gamma < 0 || beta > 1 || beta < 0 || alpha > 1 || alpha < 0)
	{
		return std::nullopt;
	}
	*/

	// If any vertex is within the sphere
	if (length(v0) > 1 || length(v1) > 1 || length(v2) > 1)
	{
		return std::nullopt;
	}

	std::cout << "1true" << std::endl;

	return CollisionInfo((inverse(ellipsoidInvMatrix)) * glm::vec4(intersectionPointonPlane, 1), TRIANGLE, ELLIPSOID);
}