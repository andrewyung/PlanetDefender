#include <iostream>
#include <filesystem>

#include "AppDriver.h"
#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"
#include "ShaderLoader.h"

using namespace std;

int main(int argc, char **argv)
{
	WindowCanvas canvas;
	FileOperations fileOp;

	canvas.initializeWindow(argc, argv);
	try
	{
		std::string defaultVertex = fileOp.readFile("shaders/DefaultVertex.vs");
		std::string  defaultFragment = fileOp.readFile("shaders/DefaultFragment.fs");
		ShaderLoader shader;
		GLuint shaderID = shader.load(defaultVertex.c_str(), defaultFragment.c_str());
		canvas.setDefaultShader(shaderID);
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;
	}

	ModelLoader modelLoader;
	Model* model = modelLoader.createPrimitive(modelLoader.CUBE);
	std::cout << "Model data : " << "indices - " << model->indexData.size() << " : " << "vertices - " << model->vertexData.size() << std::endl;
	canvas.addModel(*model);

	canvas.start();

	return 0;
}
