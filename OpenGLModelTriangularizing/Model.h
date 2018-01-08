#pragma once
#include <vector>

#include "ShaderLoader.h"
#include "Vertex.h"

class Model
{
public:
	Model(std::vector<Vertex> &vertexData, std::vector<int> &indexData) : vertexData(vertexData), indexData(indexData) {};
	~Model();

	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	GLuint shader;
};

