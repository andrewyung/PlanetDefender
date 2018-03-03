#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

class Light : public Model
{
public:
	glm::vec3 lightColor = glm::vec3();
	float strength = 1;

	Light(std::vector<Vertex> &vertexData, std::vector<int> &indexData, glm::vec3 color, float strength) : Model(vertexData, indexData)
	{
		calculateCentroid();

		lightColor = color;
		this->strength = strength;
	}
	Light(std::vector<Vertex> &vertexData, std::vector<int> &indexData) : Model(vertexData, indexData)
	{
		calculateCentroid();
	}

	//Returned position is in world position
	glm::vec4 getLightPosition()
	{
		//could possible use w for extra data
		return WindowCanvas::getCurrentCameraModelMatrix() * Model::Model::vaoInfo->transformation * glm::vec4(lightPosition, strength);
	}

private:
	glm::vec3 lightPosition = glm::vec3();

	void calculateCentroid()
	{
		//set light position as the average of all vertices in model
		glm::vec3 vertexMean;

		for (int i = 0; i < vertexData.size(); i++)
		{
			vertexMean += glm::vec3(vertexData[i].x, vertexData[i].y, vertexData[i].z);
		}
		vertexMean /= vertexData.size();

		lightPosition = vertexMean;
	}
};

