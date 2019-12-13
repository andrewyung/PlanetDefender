#pragma once

#include <iostream>
#include <filesystem>

#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"
#include "Particles.h"
#include "Light.h"

using namespace std;

WindowCanvas canvas;

GLuint shaderID, shaderID1, shaderID2, diffuseShader;

ModelLoader modelLoader;
Model* model1 = modelLoader.createPrimitive(modelLoader.TRIANGLE);
Model* model2 = modelLoader.createPrimitive(modelLoader.QUAD);

Model* model6 = modelLoader.createPrimitive(modelLoader.CUBE);

//using different shaders
Model* model4 = modelLoader.createPrimitive(modelLoader.CUBE);
Model* model5 = modelLoader.createPrimitive(modelLoader.CUBE);

Model* loadedModel = modelLoader.loadModel("airboat.obj");

Particles* particles1 = new Particles(modelLoader.createPrimitive(modelLoader.QUAD));
Particles* particles2 = new Particles(modelLoader.createPrimitive(modelLoader.TRIANGLE));

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
	canvas.setCamera(mainCamera);
	//translate camera to view test objects (since camera is at origin)
	mainCamera.translate(glm::vec3(0, 0, -3), true);

	//models
	canvas.addModel(*model1, false);

	canvas.addModel(*model2, false);

	model4->shader = shaderID1;
	canvas.addModel(*model4, false);

	model5->shader = shaderID2;
	canvas.addModel(*model5, false);

	canvas.addModel(*model6, false);

	//check if setting shader after addModel works
	loadedModel->shader = diffuseShader;
	canvas.addModel(*loadedModel, false);

	//particles
	std::vector<glm::mat4> particleTransforms;
	for (int i = 1; i < 2001; i++)
	{
		for (int k = 1; k < 2001; k++)
		{
			particleTransforms.push_back(glm::translate(glm::mat4(), glm::vec3(k * 1.5, i * 1.5f, 0)));
		}
	}
	canvas.addParticles(*particles1, 2000 * 2000, particleTransforms);

	particleTransforms.clear();
	for (int i = 2; i < 2002; i++)
	{
		for (int k = 1; k < 2001; k++)
		{
			particleTransforms.push_back(glm::translate(glm::mat4(), glm::vec3(k * -0.3f, i * -0.3f, 0)));
		}
	}
	canvas.addParticles(*particles2, 2000 * 2000, particleTransforms);

	//light
	Light* light1 = modelLoader.createLight();
	Light* light2 = modelLoader.createLight();
	Light* light3 = modelLoader.createLight();

	light1->lightColor = glm::vec3(0, 1, 0);
	canvas.addLight(*light1);

	light2->lightColor = glm::vec3(0, 0, 1);
	light2->strength = 5;
	canvas.addLight(*light2);

	light3->lightColor = glm::vec3(1, 0, 0);
	canvas.addLight(*light3);

	//model initial transformations
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
	model1->translate((float) WindowCanvas::deltaCallbackTime * glm::vec3(0.3f, 0.3f, 0) * (float) sin((float) WindowCanvas::frames / 10.0f) * 3.0f, false);
	model1->rotate(60 * WindowCanvas::deltaCallbackTime, glm::vec3(1.0f, 0.0f, 1.0f));

	model6->rotate(60 * WindowCanvas::deltaCallbackTime, glm::vec3(-1.0f, 0.0f, -1.0f));

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
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(0, 0, -0.3f));
	}
	else if(key == 'k')
	{
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(0, 0, 0.3f));
	}
	else if (key == 'j')
	{
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(-0.3f, 0, 0));
	}
	else if (key == 'l')
	{
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(0.3f, 0, 0));
	}
	else if (key == 'p')
	{
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(0, 0.3f, 0));
	}
	else if (key == ';')
	{
		canvas.lights[currentControlledLightIndex]->translate(glm::vec3(0, -0.3f, 0));
	}
	else if (key == 'o')
	{
		currentControlledLightIndex++;
		if (currentControlledLightIndex >= canvas.lights.size())
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
		canvas.setDefaultShader(shaderID);

		std::string changingVertex = fileOp.readFile("shaders/ChangingVertex.vs");
		std::string greenFragment = fileOp.readFile("shaders/GreenFragment.fs");
		shaderID1 = shader.load(changingVertex.c_str(), greenFragment.c_str());

		std::string expandingNormalsVertex = fileOp.readFile("shaders/ExpandingOnNormal.vs");
		shaderID2 = shader.load(expandingNormalsVertex.c_str(), defaultFragment.c_str());

		std::string defaultParticleVertex = fileOp.readFile("shaders/DefaultParticleVertex.vs");
		GLuint particleShaderID = shader.load(defaultParticleVertex.c_str(), defaultFragment.c_str());
		canvas.setDefaultParticleShader(particleShaderID);

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

	canvas.initializeWindow(argc, argv);
	
	loadShaders();
	
	//add models and assign shaders to models if desired otherwise default shader is used.
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	canvas.start(gameLoop, gameInitialization, mouseCallback, keyboardCallback, mouseMotionCallback);

	return 0;
}
