#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

class Light : public Model
{
public:
	glm::vec3 lightColor = glm::vec3();
	float intensity = 1;

	Light(std::vector<Vertex> &vertexData, std::vector<int> &indexData, glm::vec3 color, float strength) : Model(vertexData, indexData)
	{
		calculateCentroid();

		lightColor = color;
		intensity = strength;
	}
	Light(std::vector<Vertex> &vertexData, std::vector<int> &indexData) : Model(vertexData, indexData)
	{
		calculateCentroid();
	}

	//Returned position is in world position
	glm::vec4 getLightPosition()
	{
		//could possible use w for extra data
		return glm::vec4(glm::vec3(WindowCanvas::getCurrentCameraModelMatrix() * (Model::vaoInfo->rotation * (Model::vaoInfo->scale * Model::vaoInfo->translation)) * glm::vec4(lightPosition, 1)), intensity);
	}

	void scale(glm::vec3 scale, bool localScale = true)
	{
		Model::scale(scale, localScale);

		intensity += glm::length(scale);
	}

	void translate(glm::vec3 translateVector, bool localSpace = true)
	{
		Model::scale(translateVector, localSpace);

		updateLightPosition();
	}
	
	void rotate(float angle, glm::vec3 axis, bool localSpace = true)
	{
		Model::rotate(angle, axis, localSpace);

		updateLightPosition();
	}


	void updateLightPosition()
	{
		lightPosition.x = vaoInfo->translation[0][3];
		lightPosition.y = vaoInfo->translation[1][3];
		lightPosition.z = vaoInfo->translation[2][3];
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

