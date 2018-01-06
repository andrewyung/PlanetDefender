#pragma once
#include "Vertex.h"
#include <vector>

class Object
{
public:
	Object(std::vector<Vertex> vertexData, std::vector<int> indiceData) : vertexData(vertexData), indiceData(indiceData) {};
	~Object();

	std::vector<Vertex> vertexData;
	std::vector<int> indiceData;

};

