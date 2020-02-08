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
#include "RayCollider.h"

using namespace std;

GameScene scene;
WindowCanvas canvas;

int currentControlledLightIndex = 0; //used to switch between lights to move around

bool leftMouseDown = false;
bool rightMouseDown = false;
int lastMouseX, lastMouseY;

Model planet = ModelLoader::loadModel("Sphere.obj");
Model planetGreen = ModelLoader::loadModel("Sphere.obj");
Model ship = ModelLoader::loadModel("Ship.obj");

Model test = ModelLoader::createPrimitive(ModelLoader::QUAD);

Light sunLight = ModelLoader::createLight();

Model mouseRay = ModelLoader::createPrimitive(ModelLoader::CUBE);

Camera mainCamera;

//called once at the beginning
void gameInitialization()
{
	scene.loadAll();

	canvas.addSkybox(scene.skyboxTextureAlias);

	sunLight.shader = scene.lightShader;

	//camera
	canvas.setCamera(mainCamera);
	//translate camera to view test objects (since camera is at origin)
	mainCamera.translate(glm::vec3(0, 0, -4), true);


	ShaderLoader::setVector4(sunLight.shader, "emit_color", glm::vec4(0.4f, 0.8f, 1, 1));
	sunLight.lightColor = glm::vec3(100.0f/255.0f, 182.0f/255.0f, 255.0f/255.0f);
	sunLight.intensity = 0.2f;

	// Planet
	planetGreen.textures.push_back(scene.greenTiledTexture);
	planetGreen.shader = scene.diffuseShader;

	planetGreen.addColliderProperty(std::make_shared<ColliderProperties>(EllipsoidCollider()));
	canvas.addModel(planetGreen, false);
	planetGreen.translate(glm::vec3(-4, 0, 0));

	// Planet 
	planet.textures.push_back(scene.earthTexture);
	planet.textures.push_back(scene.earthNormalTexture);
	planet.textures.push_back(scene.earthSpecularTexture);
	planet.shader = scene.diffuseNormalShader;
	ShaderLoader::setVector4(planet.shader, "ambientLight", 0.05f * glm::vec4(204/255, 221/255, 255/255, 1));
	ShaderLoader::setFloat(planet.shader, "shininess", 1.0f);
	
	planet.addColliderProperty(std::make_shared<ColliderProperties>(EllipsoidCollider()));

	canvas.addModel(planet, false);
	planetGreen.addVelocity(glm::vec3(0.02, 0.03, 0));

	// Ship
	ship.shader = scene.shipShader;
	canvas.addModel(ship, false);
	ship.translate(glm::vec3(-1.5f, 0, 0));
	ship.rotate(90, glm::vec3(0, 1.0f, 0));
	ship.scale(glm::vec3(0.2f, 0.2f, 0.2f));

	// Lights
	canvas.addLight(sunLight); 
	sunLight.translate(glm::vec3(-1.8f, 0.4f, 0.0f));
	sunLight.scale(glm::vec3(0.2f, 0.2f, 0.2f));
	sunLight.setDrawing(true);
	//canvas.addModel(sunModel, false);

	//canvas.addModel(test, false);

	canvas.bloom = true;


	mouseRay.addColliderProperty(std::make_shared<ColliderProperties>(RayCollider({ 0,0,0 }, { 0,0,0 })));
	mouseRay.setDrawing(false);
	canvas.addModel(mouseRay, false);
}

//called repeatly as soon as possible
void gameLoop()
{
	sunLight.rotate(WindowCanvas::deltaCallbackTime * 28, glm::vec3(0.0f, 1.0f, 0.0f), false);
	sunLight.translate(glm::vec3(0, 0, WindowCanvas::deltaCallbackTime * 0.8f));

	if (leftMouseDown)
	{
		//std::dynamic_pointer_cast<RayCollider>(mouseRay.getColliderProperty(0))->setRay(;
	}
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
	//light movement
	if (key == 'b')
	{
		canvas.bloom = !canvas.bloom;
		std::cout << "bloom: " << canvas.bloom << std::endl;
	}
}

int main(int argc, char **argv)
{
	canvas.initializeWindow(argc, argv);
	
	//add models and assign shaders to models if desired otherwise default shader is used.
	//model1->shader = shaderID1;
	//std::cout << "Model data : " << "indices - " << model1->indexData.size() << " : " << "vertices - " << model1->vertexData.size() << std::endl;

	canvas.start(gameLoop, gameInitialization, mouseCallback, keyboardCallback, mouseMotionCallback);

	return 0;
}
