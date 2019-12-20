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
	std::vector<GLuint> textures{};

	bool drawing() const { return vaoInfo->drawing; };

	void scale(glm::vec3 scale)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->scale = glm::scale(vaoInfo->scale, scale);
		}
	}

	void translate(glm::vec3 translateVector)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->translation = glm::translate(vaoInfo->translation, translateVector);
		}
	}
	void translate(glm::vec3 translateVector, bool localSpace = true)
	{
		if (vaoInfo != nullptr)
		{
			if (!localSpace)
			{
				vaoInfo->translation = glm::translate(glm::mat4(), translateVector) * vaoInfo->translation;
			}
			else
			{
				vaoInfo->translation = glm::translate(vaoInfo->translation, translateVector);
			}
		}
	}

	void rotate(float angle, glm::vec3 axis)
	{
		if (vaoInfo != nullptr)
		{
			vaoInfo->rotation = glm::rotate(vaoInfo->rotation, glm::radians(angle), axis);
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

