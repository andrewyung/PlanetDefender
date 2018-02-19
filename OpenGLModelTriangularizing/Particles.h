#pragma once

#include <vector>

#include "Model.h"
#include "WindowCanvas.h"

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

