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
	
		GLuint shaderID1;
	try
	{
		std::string defaultVertex = fileOp.readFile("shaders/DefaultVertex.vs");
		std::string defaultFragment = fileOp.readFile("shaders/DefaultFragment.fs");
		ShaderLoader shader;
		GLuint shaderID = shader.load(defaultVertex.c_str(), defaultFragment.c_str());
		canvas.setDefaultShader(shaderID);

		std::string greenFragment = fileOp.readFile("shaders/GreenFragment.fs");
		shaderID1 = shader.load(defaultVertex.c_str(), greenFragment.c_str());
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;
	}

	ModelLoader modelLoader;
	Model* model1 = modelLoader.createPrimitive(modelLoader.TRIANGLE);
	Model* model2 = modelLoader.createPrimitive(modelLoader.QUAD);
	Model* model3 = modelLoader.createPrimitive(modelLoader.CUBE);
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;
	canvas.addModel(*model1, false);
	canvas.addModel(*model2, false);
	canvas.addModel(*model3, false);

	canvas.start();

	return 0;
}
