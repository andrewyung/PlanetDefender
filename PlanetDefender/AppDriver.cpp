#pragma once

#include <iostream>
#include <filesystem>

#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"
#include "Particles.h"
#include "Light.h"
#include "Triangularization.h"

using namespace std;

GLuint shaderID, shaderID1, shaderID2, diffuseShader;

Model* model1 = ModelLoader::createPrimitive(ModelLoader::TRIANGLE);
//Model* model2 = ModelLoader::createPrimitive(ModelLoader::QUAD);
Model* model2 = ModelLoader::loadModel("polygon.obj");

Model* model6 = ModelLoader::createPrimitive(ModelLoader::CUBE);

//using different shaders
Model* model4 = ModelLoader::createPrimitive(ModelLoader::CUBE);
Model* model5 = ModelLoader::createPrimitive(ModelLoader::CUBE);

vector<Model> triangles;

Model* loadedModel = ModelLoader::loadModel("airboat.obj");

Particles* particles1 = new Particles(ModelLoader::createPrimitive(ModelLoader::QUAD));
Particles* particles2 = new Particles(ModelLoader::createPrimitive(ModelLoader::TRIANGLE));

Camera mainCamera;

int currentControlledLightIndex = 0; //used to switch between lights to move around

bool leftMouseDown = false;
bool rightMouseDown = false;
int lastMouseX;
int lastMouseY;

//called once at the beginning
void gameInitialization()
{
	//camera
	WindowCanvas::setCamera(mainCamera);
	//translate camera to view test objects (since camera is at origin)
	mainCamera.translate(glm::vec3(0, 0, -3), true);

	//models
	WindowCanvas::addModel(*model1, false);

	model4->shader = shaderID1;
	WindowCanvas::addModel(*model4, false);

	model5->shader = shaderID2;
	WindowCanvas::addModel(*model5, false);

	WindowCanvas::addModel(*model6, false);

	//check if setting shader after addModel works
	loadedModel->shader = diffuseShader;
	WindowCanvas::addModel(*loadedModel, false);

	//particles
	std::vector<glm::mat4> particleTransforms;
	for (int i = 1; i < 2001; i++)
	{
		for (int k = 1; k < 2001; k++)
		{
			particleTransforms.push_back(glm::translate(glm::mat4(), glm::vec3(k * 1.5, i * 1.5f, 0)));
		}
	}
	//WindowCanvas::addParticles(*particles1, 2000 * 2000, particleTransforms);

	particleTransforms.clear();
	for (int i = 2; i < 2002; i++)
	{
		for (int k = 1; k < 2001; k++)
		{
			particleTransforms.push_back(glm::translate(glm::mat4(), glm::vec3(k * -0.3f, i * -0.3f, 0)));
		}
	}
	WindowCanvas::addParticles(*particles2, 2000 * 2000, particleTransforms);

	//light
	Light* light1 = ModelLoader::createLight();
	Light* light2 = ModelLoader::createLight();
	Light* light3 = ModelLoader::createLight();

	light1->lightColor = glm::vec3(0, 1, 0);
	light1->strength = 3;
	WindowCanvas::addLight(*light1);

	light2->lightColor = glm::vec3(0, 0, 1);
	light2->strength = 5;
	WindowCanvas::addLight(*light2);

	light3->lightColor = glm::vec3(1, 0, 0);
	WindowCanvas::addLight(*light3);

	//model initial transformations
	model1->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model1->translate(glm::vec3(2.0f, 2.0f, 0));

	model6->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model6->translate(glm::vec3(0.0f, 2.0f, 0));

	model4->scale(glm::vec3(0.2f, 0.2f, 0.2f));

	model5->scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model5->translate(glm::vec3(0.0f, -2.0f, 0));

	// Triangularize
	
	WindowCanvas::addModel(*model2, false);
	model2->scale(glm::vec3(2.0f, 2.0f, 2.0f));
	model2->setDrawing(false);

	triangles = Triangularization::EarTriangularize(*model2);

	for (int i = 0; i < triangles.size(); i++)
	{
		WindowCanvas::addModel(triangles[i], false);
		triangles[i].translate(glm::vec3(0.0f, 1.5f, 0.0f));
	}
}

//called repeatly as soon as possible
void gameLoop()
{
	model1->translate((float) WindowCanvas::deltaCallbackTime * glm::vec3(0.3f, 0.3f, 0) * (float) sin((float) WindowCanvas::frames / 10.0f) * 3.0f, false);
	model1->rotate(60 * WindowCanvas::deltaCallbackTime, glm::vec3(1.0f, 0.0f, 1.0f));

	model6->rotate(60 * WindowCanvas::deltaCallbackTime, glm::vec3(-1.0f, 0.0f, -1.0f));

	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].translate((float)WindowCanvas::deltaCallbackTime * glm::vec3(0.0f, i * 0.07f, 0.0f) * (sin((float)WindowCanvas::frames / 100.0f)), false);
	}

	/*
	//std::cout << WindowCanvas::frames << std::endl;
	if (WindowCanvas::frames > 200)
	{
		loadedModel->setDrawing(false);
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames / 10000.0f << std::endl;
	}
	if (WindowCanvas::frames > 400)
	{
		loadedModel->setDrawing(true);
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames / 10000.0f << std::endl;
	*/
}

void mouseCallback(int button, int state, int x, int y)
{
	std::cout << x << " : " << y << std::endl;
	if ((button == 3) || (button == 4)) // scroll wheel event
	{
		if (button == 3)
		{
			//mainCamera.ModelMatrix = glm::translate(mainCamera.ModelMatrix, glm::vec3(0, -0.1, 0));
			mainCamera.translate(glm::vec3(0, 0, 0.1), true);
		}
		else
		{
			//mainCamera.ModelMatrix = glm::translate(mainCamera.ModelMatrix, glm::vec3(0, 0.1, 0));
			mainCamera.translate(glm::vec3(0, 0, -0.1), true);
		}
	}
	else if (button == 0)
	{
		if (state == GLUT_DOWN)
		{
			leftMouseDown = true;
			lastMouseX = x;
			lastMouseY = y;
		}
		else
		{
			leftMouseDown = false;
		}
	}
	else if (button == 2)
	{
		if (state == GLUT_DOWN)
		{
			rightMouseDown = true;
			lastMouseX = x;
			lastMouseY = y;
		}
		else
		{
			rightMouseDown = false;
		}
	}
}

void mouseMotionCallback(int x, int y)
{
	if (leftMouseDown)
	{
		mainCamera.translate(glm::vec3(x - lastMouseX, lastMouseY - y, 0) * WindowCanvas::deltaCallbackTime, true);
	}
	if (rightMouseDown)
	{
		mainCamera.rotate(glm::vec3(x - lastMouseX, 0, 0), true);
		mainCamera.rotate(glm::vec3(0, y - lastMouseY, 0), true);
	}
	if (leftMouseDown || rightMouseDown)
	{
		lastMouseX = x;
		lastMouseY = y;
	}

}

void keyboardCallback(unsigned char key, int x, int y)
{
	//rotate doesnt currently work
	if (key == 's')
	{
		mainCamera.rotate(glm::vec3(0, 5, 0));
		std::cout << WindowCanvas::deltaCallbackTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'w')
	{
		mainCamera.rotate(glm::vec3(0, -5, 0));
	}
	
	if (key == 'd')
	{
		mainCamera.rotate(glm::vec3(5, 0, 0));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'a')
	{
		mainCamera.rotate(glm::vec3(-5, 0, 0));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;
	}

	if (key == 'q')
	{
		mainCamera.rotate(glm::vec3(0, 0, -5));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'e')
	{
		mainCamera.rotate(glm::vec3(0, 0, 5));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;
	}

	//light movement
	if (key == 'i')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(0, 0, -0.3f));
	}
	else if(key == 'k')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(0, 0, 0.3f));
	}
	else if (key == 'j')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(-0.3f, 0, 0));
	}
	else if (key == 'l')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(0.3f, 0, 0));
	}
	else if (key == 'p')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(0, 0.3f, 0));
	}
	else if (key == ';')
	{
		WindowCanvas::lights[currentControlledLightIndex]->translate(glm::vec3(0, -0.3f, 0));
	}
	else if (key == 'o')
	{
		currentControlledLightIndex++;
		if (currentControlledLightIndex >= WindowCanvas::lights.size())
		{
			currentControlledLightIndex = 0;
		}
	}
}

void loadShaders()
{
	FileOperations fileOp;

	//load shaders
	try
	{
		std::string defaultVertex = fileOp.readFile("shaders/DefaultVertex.vs");
		std::string defaultFragment = fileOp.readFile("shaders/DefaultFragment.fs");
		ShaderLoader shader;
		shaderID = shader.load(defaultVertex.c_str(), defaultFragment.c_str());
		//set a shader for models to use if not set
		WindowCanvas::setDefaultShader(shaderID);

		std::string changingVertex = fileOp.readFile("shaders/ChangingVertex.vs");
		std::string greenFragment = fileOp.readFile("shaders/GreenFragment.fs");
		shaderID1 = shader.load(changingVertex.c_str(), greenFragment.c_str());

		std::string expandingNormalsVertex = fileOp.readFile("shaders/ExpandingOnNormal.vs");
		shaderID2 = shader.load(expandingNormalsVertex.c_str(), defaultFragment.c_str());

		std::string defaultParticleVertex = fileOp.readFile("shaders/DefaultParticleVertex.vs");
		GLuint particleShaderID = shader.load(defaultParticleVertex.c_str(), defaultFragment.c_str());
		WindowCanvas::setDefaultParticleShader(particleShaderID);

		std::string diffuseShaderVertex = fileOp.readFile("shaders/DiffuseVert.vs");
		std::string diffueShaderFragment = fileOp.readFile("shaders/DiffuseFrag.fs");
		diffuseShader = shader.load(diffuseShaderVertex.c_str(), diffueShaderFragment.c_str());
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;

		system("pause");
	}
}



int main(int argc, char **argv)
{

	WindowCanvas canvas;

	WindowCanvas::initializeWindow(argc, argv);
	
	loadShaders();
	
	//add models and assign shaders to models if desired otherwise default shader is used.
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	WindowCanvas::start(gameLoop, gameInitialization, mouseCallback, keyboardCallback, mouseMotionCallback);

	return 0;
}
