#include "Light.h"

Light::Light(glm::vec3 position)
{
	Light::lightPosition = position;
}

// translate in local space
void Light::translate(glm::vec3 translation)
{
	Light::lightPosition = glm::translate(glm::mat4(), translation) * glm::vec4(Light::lightPosition, 1.0);
}

Light::~Light()
{
}
