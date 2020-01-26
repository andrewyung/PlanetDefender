#pragma once

#include <vector>
#include <string>

#include "Vertex.h"
#include "WindowCanvas.h"

class Model
{
public:
	Model(std::vector<Vertex> vertexData, std::vector<int> indexData) :
		vertexData(vertexData), indexData(indexData) {};

	std::string name;

	std::vector<Vertex> vertexData;
	std::vector<int> indexData;
	
	GLuint shader;	
	std::vector<GLuint> textures{};

	bool drawing() const { return vaoInfo->drawing; };

	virtual void scale(glm::vec3 scale, bool localScale = true)
	{
		if (vaoInfo == nullptr) return;

		if (localScale)
		{
			vaoInfo->scale = glm::scale(vaoInfo->scale, scale);
		}
		else
		{
			vaoInfo->scale = glm::scale(glm::mat4(), scale);
		}
	}

	virtual void translate(glm::vec3 translateVector, bool localSpace = true)
	{
		if (vaoInfo == nullptr) return;

		if (localSpace)
		{
			vaoInfo->translation = vaoInfo->translation * glm::translate(glm::mat4(), glm::mat3(vaoInfo->rotation) * translateVector);
		}
		else
		{
			vaoInfo->translation = glm::translate(glm::mat4(), translateVector) * vaoInfo->translation;
		}
	}

	void addVelocity(glm::vec3 veloVector, bool localSpace = true)
	{
		if (vaoInfo == nullptr) return;

		if (localSpace)
		{
			vaoInfo->velocity += glm::vec3(vaoInfo->rotation * glm::vec4(veloVector, 0));
		}
		else
		{
			vaoInfo->velocity = veloVector;
		}
	}
	glm::vec3 getVelocity()
	{
		return vaoInfo->velocity;
	}

	virtual void rotate(float angle, glm::vec3 axis, bool localSpace = true)
	{
		if (vaoInfo == nullptr) return;

		if (localSpace)
		{
			vaoInfo->rotation = vaoInfo->rotation * glm::rotate(glm::mat4(), glm::radians(angle), axis);
		}
		else
		{
			vaoInfo->rotation = glm::rotate(glm::mat4(), glm::radians(angle), axis) * vaoInfo->rotation;
		}
	}

	void setDrawing(bool toBeDrawn)
	{
		if (vaoInfo == nullptr) return;

		vaoInfo->drawing = toBeDrawn;
	}

	void addColliderProperty(std::shared_ptr<ColliderProperties> prop)
	{
		colliderProperties.push_back(prop);
	}
	void removeColliderProperty(int index)
	{
		colliderProperties.erase(colliderProperties.begin() + index);
	}
	std::shared_ptr<ColliderProperties> getColliderProperty(int index)
	{
		return colliderProperties[index];
	}

	std::vector<float> getCenter()
	{
		std::vector<float> center{ 0, 0, 0 };
		for (int i = 0; i < vertexData.size(); i++)
		{
			center[0] += vertexData[i].x;
			center[1] += vertexData[i].y;
			center[2] += vertexData[i].z;
		}

		center[0] /= vertexData.size();
		center[1] /= vertexData.size();
		center[2] /= vertexData.size();

		return center;
	}

	std::shared_ptr<VAOInfo> getVAOInfo()
	{
		return vaoInfo;
	}

protected:
	//Information used by WindowCanvas to manage Model
	GLsizeiptr vertexDataOffset = -1;
	GLsizeiptr indexDataOffset = -1;
	std::shared_ptr<VAOInfo> vaoInfo;

	std::vector<std::shared_ptr<ColliderProperties>> colliderProperties;

	void setVertexBufferAndArrayData(GLsizeiptr vertexDataOffset, GLsizeiptr indexDataOffset)
	{
		this->vertexDataOffset = vertexDataOffset;
		this->indexDataOffset = indexDataOffset;
	}

	void setVAOInfo(std::shared_ptr<VAOInfo> index)
	{
		vaoInfo = index;
	}

	friend class WindowCanvas;
};

