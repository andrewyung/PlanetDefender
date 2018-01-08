#pragma once
#include "Model.h"

class ModelLoader
{
public:
	enum PrimitiveTypes {
		QUAD,
		TRIANGLE,
		CUBE
		//add more later
	};

	static Model* createPrimitive(PrimitiveTypes type);

	~ModelLoader();
};