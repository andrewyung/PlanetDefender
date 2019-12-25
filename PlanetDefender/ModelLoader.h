#pragma once

#include "Model.h"
#include "Light.h"

class ModelLoader
{
public:
	enum PrimitiveTypes {
		QUAD,
		TRIANGLE,
		CUBE,
		SKYBOX_CUBE
		//add more later
	};

	static Model* createPrimitive(PrimitiveTypes type);

	static Light * createLight();

	static Model* loadModel(std::string objPath);

	~ModelLoader();
};