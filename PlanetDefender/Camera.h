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
	glm::vec3 getCameraPos();

	void translate(glm::vec3 translation, bool localSpace = true);

	void rotate(glm::vec3 rotation, bool localSpace = true);

	Camera();
	~Camera();
private:
	void printMVP(); // Debugging purposes
};

