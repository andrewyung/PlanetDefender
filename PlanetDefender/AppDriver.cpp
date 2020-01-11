#pragma once

#include <iostream>
#include <filesystem>

#include "GameScene.h"
#include "WindowCanvas.h"
#include "ModelLoader.h"
#include "FileOperations.h"
#include "Particles.h"
#include "Light.h"
#include "Triangularization.h"
#include "stb_image.h"
#include "TextureLoader.h"
#include "EllipsoidCollider.h"

using namespace std;

GameScene *scene = new GameScene();

int currentControlledLightIndex = 0; //used to switch between lights to move around

bool leftMouseDown = false;
bool rightMouseDown = false;
int lastMouseX, lastMouseY;

//called once at the beginning
void gameInitialization()
{
	scene->loadTextures();
	scene->loadShaders();

	WindowCanvas::addSkybox(scene->skyboxTextureAlias);

	scene->planet = ModelLoader::loadModel("Sphere.obj");

	scene->test = ModelLoader::createPrimitive(ModelLoader::QUAD);

	//camera
	WindowCanvas::setCamera(scene->mainCamera);
	//translate camera to view test objects (since camera is at origin)
	scene->mainCamera.translate(glm::vec3(0, 0, -3), true);

	scene->planet->textures.push_back(scene->greenTiledTexture);
	scene->planet->shader = scene->diffuseShader;

	scene->planet->addColliderProperty(std::make_shared<ColliderProperties>(EllipsoidCollider()));

	WindowCanvas::addModel(*(scene->planet), false);
	scene->planet->addVelocity(glm::vec3(0, 0.1, 0));

	WindowCanvas::addModel(*(scene->test), false);
}

//called repeatly as soon as possible
void gameLoop()
{
	//scene->test->rotate(WindowCanvas::deltaCallbackTime * 90, glm::vec3(1.0f, 0.0f, 0.0f), false);

	//scene->test->translate(glm::vec3(0, WindowCanvas::deltaCallbackTime * 5, 0), false);
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
			scene->mainCamera.translate(glm::vec3(0, 0, 0.1), true);
		}
		else
		{
			//mainCamera.ModelMatrix = glm::translate(mainCamera.ModelMatrix, glm::vec3(0, 0.1, 0));
			scene->mainCamera.translate(glm::vec3(0, 0, -0.1), true);
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
		scene->mainCamera.translate(glm::vec3(x - lastMouseX, lastMouseY - y, 0) * WindowCanvas::deltaCallbackTime, true);
	}
	if (rightMouseDown)
	{
		scene->mainCamera.rotate(glm::vec3(x - lastMouseX, 0, 0), true);
		scene->mainCamera.rotate(glm::vec3(0, y - lastMouseY, 0), true);
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
		scene->mainCamera.rotate(glm::vec3(0, 5, 0));
		std::cout << WindowCanvas::deltaCallbackTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'w')
	{
		scene->mainCamera.rotate(glm::vec3(0, -5, 0));
	}
	
	if (key == 'd')
	{
		scene->mainCamera.rotate(glm::vec3(5, 0, 0));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'a')
	{
		scene->mainCamera.rotate(glm::vec3(-5, 0, 0));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;
	}

	if (key == 'q')
	{
		scene->mainCamera.rotate(glm::vec3(0, 0, -5));
		//std::cout << WindowCanvas::deltaFrameTime << " : " << (float)WindowCanvas::frames << std::endl;

	}
	else if (key == 'e')
	{
		scene->mainCamera.rotate(glm::vec3(0, 0, 5));
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

int main(int argc, char **argv)
{

	WindowCanvas canvas;

	WindowCanvas::initializeWindow(argc, argv);
	
	//add models and assign shaders to models if desired otherwise default shader is used.
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	WindowCanvas::start(gameLoop, gameInitialization, mouseCallback, keyboardCallback, mouseMotionCallback);

	return 0;
}
