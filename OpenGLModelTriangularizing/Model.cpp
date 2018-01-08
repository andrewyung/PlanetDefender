#include "Model.h"
#include <vector>
#include "WindowCanvas.h"

Model::Model(std::vector<Vertex> &vertexData, std::vector<int> &indexData)
{
	WindowCanvas canvas;
	modelID = canvas.assignModelID();
}
Model::~Model()
{
}
