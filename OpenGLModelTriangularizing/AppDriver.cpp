#include <iostream>
#include <filesystem>

#include "AppDriver.h"
#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"

using namespace std;

WindowCanvas canvas;

ModelLoader modelLoader;
Model* model1 = modelLoader.createPrimitive(modelLoader.TRIANGLE);
Model* model2 = modelLoader.createPrimitive(modelLoader.QUAD);
Model* model3 = modelLoader.createPrimitive(modelLoader.CUBE);

Model* model6 = modelLoader.createPrimitive(modelLoader.CUBE);

//using different shaders
Model* model4 = modelLoader.createPrimitive(modelLoader.CUBE);
Model* model5 = modelLoader.createPrimitive(modelLoader.CUBE);

//called once at the beginning
void gameInitialization()
{
	model2->scale(glm::vec3(0.1f, 0.1f, 0.1f));

	model1->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model1->translate(glm::vec3(2.0f, 2.0f, 0));

	model6->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model6->translate(glm::vec3(0.0f, 2.0f, 0));

	model4->scale(glm::vec3(0.2f, 0.2f, 0.2f));

	model5->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model5->translate(glm::vec3(0.0f, -2.0f, 0));
}

//called repeatly as soon as possible
void gameLoop()
{
	model1->translate((float) WindowCanvas::deltaFrameTime * glm::vec3(0.3f, 0.3f, 0) * (float) sin((float) WindowCanvas::frames / 10.0f) * 3.0f, false);
	model1->rotate(60 * WindowCanvas::deltaFrameTime, glm::vec3(1.0f, 0.0f, 1.0f));

	model6->rotate(60 * WindowCanvas::deltaFrameTime, glm::vec3(-1.0f, 0.0f, -1.0f));
	if (WindowCanvas::deltaFrameTime != 0)
	{
		std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames / 10000.0f << std::endl;
	}
}

int main(int argc, char **argv)
{
	WindowCanvas canvas;
	FileOperations fileOp;

	canvas.initializeWindow(argc, argv);
	
	GLuint shaderID, shaderID1, shaderID2;
	try
	{
		std::string defaultVertex = fileOp.readFile("shaders/DefaultVertex.vs");
		std::string defaultFragment = fileOp.readFile("shaders/DefaultFragment.fs");
		ShaderLoader shader;
		shaderID = shader.load(defaultVertex.c_str(), defaultFragment.c_str());
		canvas.setDefaultShader(shaderID);

		std::string changingVertex = fileOp.readFile("shaders/ChangingVertex.vs");
		std::string greenFragment = fileOp.readFile("shaders/GreenFragment.fs");
		shaderID1 = shader.load(changingVertex.c_str(), greenFragment.c_str());

		std::string expandingNormalsVertex = fileOp.readFile("shaders/ExpandingOnNormal.vs");
		shaderID2 = shader.load(expandingNormalsVertex.c_str(), defaultFragment.c_str());
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;
		return 0;
	}
	
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	canvas.addModel(*model3, true);
	canvas.addModel(*model1, true);

	canvas.addModel(*model2, false);

	model4->shader = shaderID1;
	canvas.addModel(*model4, false);

	model5->shader = shaderID2;
	canvas.addModel(*model5, false);

	canvas.addModel(*model6, false);

	canvas.start(gameLoop, gameInitialization);

	return 0;
}
