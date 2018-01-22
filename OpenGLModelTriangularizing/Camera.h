#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::mat4 ModelMatrix = glm::mat4();
	glm::mat4 ViewMatrix = glm::mat4();
	glm::mat4 ProjectionMatrix = glm::mat4();

	glm::mat4 getMVP();

	void translate(glm::vec3 translation);

	void rotate(glm::vec3 rotation);

	Camera();
	~Camera();
};

