#pragma once

#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Camera::getMVP()
{
	return ProjectionMatrix * ViewMatrix * ModelMatrix;
}

glm::vec3 Camera::getCameraPos()
{
	glm::mat3 rotationMat(ViewMatrix);
	glm::vec3 pos(ViewMatrix[3]);

	return -pos * rotationMat;
}

void Camera::printMVP()
{
	std::cout << "PROJECTION: " << glm::to_string(ProjectionMatrix) << std::endl;
	std::cout << "VIEW: " << glm::to_string(ViewMatrix) << std::endl;
	std::cout << "MODEL: " << glm::to_string(ModelMatrix) << std::endl;
}

// default to local space but allow option for world space
void Camera::translate(glm::vec3 translation, bool localspace)
{
	if (localspace)
	{
		Camera::ViewMatrix = glm::translate(glm::mat4(), translation) * Camera::ViewMatrix;

	}
	else
	{
		Camera::ViewMatrix = Camera::ViewMatrix * glm::translate(glm::mat4(), translation);
	}
}

void Camera::rotate(glm::vec3 rotation, bool localspace)
{
	if (localspace)
	{
		Camera::ViewMatrix =	glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(0, 1, 0)) *
								glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(1, 0, 0)) *
								glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
								Camera::ViewMatrix;
	}
	else
	{
		Camera::ViewMatrix =	glm::rotate(Camera::ViewMatrix, glm::radians(rotation.x), glm::vec3(0, 1, 0)) *
								glm::rotate(Camera::ViewMatrix, glm::radians(rotation.y), glm::vec3(1, 0, 0)) *
								glm::rotate(Camera::ViewMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	}
}

Camera::Camera()
{
	//fov (radians), aspect ratio, near clip, far clip
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)1600 / (float)900, 0.1f, 100.0f);
}


Camera::~Camera()
{
}
