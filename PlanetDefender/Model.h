#pragma once

#include <vector>

#include "Vertex.h"
#include "WindowCanvas.h"

class Model
{
public:
	Model(std::vector<Vertex> &vertexData, std::vector<int> &indexData) :
		vertexData(vertexData), indexData(indexData) {};

	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	GLuint shader;	
	std::vector<GLuint> textures{};

	bool drawing() const { return vaoInfo->drawing; };

	void scale(glm::vec3 scale, bool localScale = true)
	{
		if (vaoInfo != nullptr)
		{
			if (localScale)
			{
				vaoInfo->scale = glm::scale(vaoInfo->scale, scale);
			}
			else
			{
				vaoInfo->scale = glm::scale(glm::mat4(), scale);
			}
			vaoInfo->transformUpdated = true;
		}
	}

	void translate(glm::vec3 translateVector, bool localSpace = true)
	{
		if (vaoInfo != nullptr)
		{
			if (localSpace)
			{
				vaoInfo->translation = vaoInfo->translation * glm::translate(glm::mat4(), glm::mat3(vaoInfo->rotation) * translateVector);
			}
			else
			{
				vaoInfo->translation = glm::translate(glm::mat4(), translateVector) * vaoInfo->translation;
			}
			vaoInfo->transformUpdated = true;
		}
	}

	void rotate(float angle, glm::vec3 axis, bool localSpace = true)
	{
		if (vaoInfo != nullptr)
		{
			if (localSpace)
			{
				vaoInfo->rotation = vaoInfo->rotation * glm::rotate(glm::mat4(), glm::radians(angle), axis);
			}
			else
			{
				vaoInfo->rotation = glm::rotate(glm::mat4(), glm::radians(angle), axis) * vaoInfo->rotation;
			}
			vaoInfo->transformUpdated = true;
		}
	}

	void setDrawing(bool toBeDrawn)
	{
		if (vaoInfo == nullptr)
		{
			std::cout << "Model has not been added" << std::endl;
			return;
		}
		this->vaoInfo->drawing = toBeDrawn;
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

protected:
	//Information used by WindowCanvas to manage Model
	GLsizeiptr vertexDataOffset = -1;
	GLsizeiptr indexDataOffset = -1;
	VAOInfo *vaoInfo;

	void setVertexBufferAndArrayData(GLsizeiptr vertexDataOffset, GLsizeiptr indexDataOffset)
	{
		this->vertexDataOffset = vertexDataOffset;
		this->indexDataOffset = indexDataOffset;
	}

	void setVAOInfo(VAOInfo &index)
	{
		this->vaoInfo = &index;
	}

	friend class WindowCanvas;
};

