#include "Camera.h"
#include<iostream>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Camera::getMVP()
{
	return ProjectionMatrix * ViewMatrix * ModelMatrix;
}

void Camera::translate(glm::vec3 translation)
{
	std::cout << "PROJECTION: " << glm::to_string(ProjectionMatrix) << std::endl;
	std::cout << "VIEW: " << glm::to_string(ViewMatrix) << std::endl;
	std::cout << "MODEL: " << glm::to_string(ModelMatrix) << std::endl;
	Camera::ModelMatrix = glm::translate(Camera::ModelMatrix, translation);
}

void Camera::rotate(glm::vec3 rotation)
{
	//maybe just construct a matrix instead of rotate for each axis
	Camera::ModelMatrix *= 
		(glm::rotate(Camera::ModelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0))*
		 glm::rotate(Camera::ModelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0))*
		 glm::rotate(Camera::ModelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1)));
}



Camera::Camera()
{
}


Camera::~Camera()
{
}
