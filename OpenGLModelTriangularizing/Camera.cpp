#include "Camera.h"
#include<iostream>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Camera::getMVP()
{
	return ProjectionMatrix * ViewMatrix * ModelMatrix;
}

void Camera::printMVP()
{
	std::cout << "PROJECTION: " << glm::to_string(ProjectionMatrix) << std::endl;
	std::cout << "VIEW: " << glm::to_string(ViewMatrix) << std::endl;
	std::cout << "MODEL: " << glm::to_string(ModelMatrix) << std::endl;
}

// translate in local space
void Camera::translate(glm::vec3 translation)
{
	printMVP();
	Camera::ModelMatrix = Camera::ModelMatrix * glm::translate(glm::mat4(), translation);
}
// default to local space but allow option for world space
void Camera::translate(glm::vec3 translation, bool worldSpace)
{
	if (worldSpace)
	{
		Camera::ModelMatrix = glm::translate(glm::mat4(), translation) * Camera::ModelMatrix;
	}
	else
	{
		translate(translation);
	}
}

//rotation in local space
void Camera::rotate(glm::vec3 rotation)
{
	Camera::ModelMatrix =	Camera::ModelMatrix *
							glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(0, 1, 0)) *
							glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(1, 0, 0)) *  
							glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0, 0, 1));
}

void Camera::rotate(glm::vec3 rotation, bool worldSpace)
{
	if (worldSpace)
	{
		Camera::ModelMatrix =	glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(0, 1, 0)) *
								glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(1, 0, 0)) *
								glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
								Camera::ModelMatrix;
	}
	else
	{
		rotate(rotation);
	}
}

Camera::Camera()
{
	//fov (radians), aspect ratio, near clip, far clip
	ProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
}


Camera::~Camera()
{
}
