#include "Camera.h"



glm::mat4 Camera::getMVP()
{
	return ProjectionMatrix * ViewMatrix * ModelMatrix;
}

Camera::Camera()
{
}


Camera::~Camera()
{
}
