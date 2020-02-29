#pragma once
#include "WindowCanvas.h"

glm::vec3 mapToHemisphere(float e, float x, float y) {
	float cos_phi = glm::cos(2.0 * glm::pi<float>() * x);
	float sin_phi = glm::sin(2.0 * glm::pi<float>() * x);
	float cos_theta = pow((1.0 - y), 1.0 / (e + 1.0));
	float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	float pu = sin_theta * cos_phi;
	float pv = sin_theta * sin_phi;
	float pw = cos_theta;
	return { pu, pv, pw };
}