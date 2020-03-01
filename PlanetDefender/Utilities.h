#pragma once
#include <utility>

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

std::pair<float, glm::vec3> getRotationAxisAngle(glm::mat4 r)
{
	float angle = glm::acos((r[0][0] + r[1][1] + r[2][2] - 1) / 2);

	glm::vec3 axis{
		r[2][1] - r[1][2] / sqrt(glm::exp2(r[2][1] - r[1][2]) + glm::exp2(r[0][2] - r[2][0]) + glm::exp2(r[1][0] - r[0][1])),
		r[0][2] - r[2][0] / sqrt(glm::exp2(r[2][1] - r[1][2]) + glm::exp2(r[0][2] - r[2][0]) + glm::exp2(r[1][0] - r[0][1])),
		r[1][0] - r[0][1] / sqrt(glm::exp2(r[2][1] - r[1][2]) + glm::exp2(r[0][2] - r[2][0]) + glm::exp2(r[1][0] - r[0][1])),
	};

	return std::make_pair(angle, axis);
}