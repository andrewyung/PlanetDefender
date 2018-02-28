#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light
{
public:
	glm::vec3 lightPosition = glm::vec3();

	Light(glm::vec3 position);
	void translate(glm::vec3 translation);
	~Light();
private:
};

