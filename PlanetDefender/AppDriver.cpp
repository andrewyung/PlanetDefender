#pragma once

#include <iostream>
#include <filesystem>

#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"
#include "Particles.h"
#include "Light.h"
#include "Triangularization.h"
#include "stb_image.h"
#include "TextureLoader.h"

using namespace std;

GLuint diffuseShader;
GLuint greenTiledTexture, skyboxTextureAlias;

Model* planet = ModelLoader::loadModel("Sphere.obj");

Model* test = ModelLoader::createPrimitive(ModelLoader::QUAD);

Camera mainCamera;

int currentControlledLightIndex = 0; //used to switch between lights to move around

bool leftMouseDown = false;
bool rightMouseDown = false;
int lastMouseX, lastMouseY;

//called once at the beginning
void gameInitialization()
{
	WindowCanvas::addSkybox(skyboxTextureAlias);

	//camera
	WindowCanvas::setCamera(mainCamera);
	//translate camera to view test objects (since camera is at origin)
	mainCamera.translate(glm::vec3(0, 0, -3), true);

	// Triangularize
	planet->textures.push_back(greenTiledTexture);
	planet->shader = diffuseShader;

	WindowCanvas::addModel(*planet, false);

	WindowCanvas::addModel(*test, false);
}

//called repeatly as soon as possible
void gameLoop()
{
	test->rotate(WindowCanvas::deltaCallbackTime * 90, glm::vec3(1.0f, 0.0f, 0.0f), false);

	test->translate(glm::vec3(0, WindowCanvas::deltaCallbackTime * 5, 0), false);
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
	//load shaders
	try
	{
		GLuint shaderID = ShaderLoader::load("shaders/DefaultVertex.vs", "shaders/DefaultFragment.fs");
		//set a shader for models to use if not set
		WindowCanvas::setDefaultShader(shaderID);

		GLuint particleShaderID = ShaderLoader::load("shaders/DefaultParticleVertex.vs", "shaders/DefaultFragment.fs");
		WindowCanvas::setDefaultParticleShader(particleShaderID);


		diffuseShader = ShaderLoader::load("shaders/DiffuseTextureVert.vs", "shaders/DiffuseTextureFrag.fs");
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;

		system("pause");
	}
}

void loadTextures()
{
	greenTiledTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/GreenTiled.jpg");
	skyboxTextureAlias = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/skyboxAlias.jpg");
}

int main(int argc, char **argv)
{

	WindowCanvas canvas;

	WindowCanvas::initializeWindow(argc, argv);
	
	loadTextures();
	loadShaders();
	
	//add models and assign shaders to models if desired otherwise default shader is used.
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	WindowCanvas::start(gameLoop, gameInitialization, mouseCallback, keyboardCallback, mouseMotionCallback);

	return 0;
}
