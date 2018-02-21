#pragma once

#include <vector>

#include "Model.h"
#include "WindowCanvas.h"

//could do inheritance as it has same transformation functions as model
class Particles
{
public:
	Particles(Model *model) :
		particleModel(model) {};

	Model* particleModel;
	std::vector<glm::mat4> particleOffsets;

	GLuint shader;

	bool drawing() const { return vaoInfo->drawing; };

	void scale(glm::vec3 scale)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->transformation = glm::scale(vaoInfo->transformation, scale);
		}
	}

	void translate(glm::vec3 translateVector)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->transformation = glm::translate(vaoInfo->transformation, translateVector);
		}
	}
	void translate(glm::vec3 translateVector, bool localSpace)
	{
		if (vaoInfo != nullptr)
		{
			if (!localSpace)
			{
				vaoInfo->transformation = glm::translate(glm::mat4(), translateVector) * vaoInfo->transformation;
			}
			else
			{
				vaoInfo->transformation = glm::translate(vaoInfo->transformation, translateVector);
			}
		}
	}

	void rotate(float angle, glm::vec3 axis)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->transformation = glm::rotate(vaoInfo->transformation, glm::radians(angle), axis);
		}
	}

	void setDrawing(bool toBeDrawn)
	{
		this->vaoInfo->drawing = toBeDrawn;
	}

private:
	VAOInfo *vaoInfo;

	void setVAOInfo(VAOInfo &index)
	{
		this->vaoInfo = &index;
	}

	friend class WindowCanvas;
};

