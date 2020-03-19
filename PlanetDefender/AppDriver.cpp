#pragma once

#include <iostream>
#include <random>
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
#include "TriangleCollider.h"
#include "RayCollider.h"
#include "Utilities.h"

using namespace std;

GameScene scene;
WindowCanvas canvas;

int shipSpeed = 12;
int spawnIntervalms = 2000;

bool leftMouseDown = false;
bool rightMouseDown = false;
int lastMouseX, lastMouseY;

Model planet = ModelLoader::loadModel("Sphere.obj");
Model planetGreen = ModelLoader::loadModel("Sphere.obj");
Model shipBlock = ModelLoader::loadModel("ShipBlock.obj");
Model shipSpike = ModelLoader::loadModel("ShipSpike.obj");

Model test = ModelLoader::createPrimitive(ModelLoader::QUAD);

Light sunLight = ModelLoader::createLight();

Model mouseRay = ModelLoader::createPrimitive(ModelLoader::CUBE);

Camera mainCamera;

vector<Model> ships;

void assignTriangleColliders(Model &model);
void spawnShip(int value);


//called once at the beginning
void gameInitialization()
{
	// Seed random
	srand(time(NULL));

	scene.loadAll();

	canvas.addSkybox(scene.skyboxTextureAlias);

	sunLight.shader = scene.lightShader;

	//camera
	canvas.setCamera(mainCamera);
	//translate camera to view test objects (since camera is at origin)
	mainCamera.translate(glm::vec3(0, 0, -5), true);


	ShaderLoader::setVector4(sunLight.shader, "emit_color", glm::vec4(0.4f, 0.8f, 1, 1));
	sunLight.lightColor = glm::vec3(100.0f/255.0f, 182.0f/255.0f, 255.0f/255.0f);
	sunLight.intensity = 0.2f;

	// Planet
	planetGreen.textures.push_back(scene.greenTiledTexture);
	planetGreen.shader = scene.diffuseShader;

	//planetGreen.addColliderProperty(std::make_shared<ColliderProperties>(EllipsoidCollider({ 0,0,0 }, { 1,1,1 })));
	canvas.addModel(planetGreen, false);
	planetGreen.translate(glm::vec3(-4, 0, 0));

	planetGreen.addVelocity(glm::vec3(0.02, 0.03, 0));

	// Planet 
	planet.textures.push_back(scene.earthTexture);
	planet.textures.push_back(scene.earthNormalTexture);
	planet.textures.push_back(scene.earthSpecularTexture);
	planet.shader = scene.diffuseNormalShader;
	ShaderLoader::setVector4(planet.shader, "ambientLight", 0.05f * glm::vec4(204/255, 221/255, 255/255, 1));
	ShaderLoader::setFloat(planet.shader, "shininess", 1.0f);
	std::shared_ptr<EllipsoidCollider> colProp = std::make_shared<EllipsoidCollider>(EllipsoidCollider({ 0,0,0 }, { 1,1,1 }));
	planet.addColliderProperty(colProp);

	canvas.addModel(planet, false);

	// Ship
	shipBlock.shader = scene.shipShader;
	assignTriangleColliders(shipBlock);

	canvas.addModel(shipBlock, false);
	shipBlock.setCollisionCallback([caller = &shipBlock](VAOInfo& source, VAOInfo& dest, CollisionInfo info) {

		std::cout << "collided!" << std::endl;
		if (info.getSourceType() == RAY && info.getDestType() == TRIANGLE)
		{
			std::cout << "raytriangle!" << std::endl;
			source.drawing = false;

			glm::mat4 mvp = caller->getVAOInfo()->translation * caller->getVAOInfo()->rotation * caller->getVAOInfo()->scale;
			glm::mat4 invTransform = glm::inverse(mvp);

			vector<Model> triangles = Triangularization::EarTriangularize(*caller, invTransform * glm::vec4(info.getCollisionPoint(), 1.0f));
			std::cout << triangles.size() << std::endl;

			for (int i{ 0 }; i < triangles.size(); i++)
			{
				canvas.addModel(triangles[i], false);
				triangles[i].setTranslation(caller->getTranslation());
				triangles[i].setRotation(caller->getRotation());
				triangles[i].setScale(caller->getScale());

				triangles[i].addVelocity((glm::vec3((mvp * glm::vec4(triangles[i].getCenter(), 1.0f))) - info.getCollisionPoint()) * 0.45f);
			}
		}
		else if (info.getSourceType() == TRIANGLE && info.getDestType() == ELLIPSOID)
		{
			std::cout << "triangleellipsoid!" << std::endl;
		}
	});

	shipBlock.translate(glm::vec3(-1.5f, 0, 0));
	shipBlock.scale(glm::vec3(0.2f, 0.2f, 0.2f));

	//Ship
	shipSpike.shader = scene.shipShader;
	assignTriangleColliders(shipSpike);

	canvas.addModel(shipSpike, false);
	shipSpike.setCollisionCallback([caller = &shipSpike](VAOInfo& source, VAOInfo& dest, CollisionInfo info) {
		std::cout << "collided!" << std::endl;
		if (info.getSourceType() == RAY && info.getDestType() == TRIANGLE)
		{
			std::cout << "raytriangle!" << std::endl;
			source.drawing = false;
		
			glm::mat4 mvp = caller->getVAOInfo()->translation * caller->getVAOInfo()->rotation * caller->getVAOInfo()->scale;
			glm::mat4 invTransform = glm::inverse(mvp);

			vector<Model> triangles = Triangularization::EarTriangularize(*caller, invTransform * glm::vec4(info.getCollisionPoint(), 1.0f));
			std::cout << triangles.size() << std::endl;

			for (int i{ 0 }; i < triangles.size(); i++)
			{
				canvas.addModel(triangles[i], false);
				triangles[i].setTranslation(caller->getTranslation());
				triangles[i].setRotation(caller->getRotation());
				triangles[i].setScale(caller->getScale());

				triangles[i].addVelocity((glm::vec3((mvp * glm::vec4(triangles[i].getCenter(), 1.0f))) - info.getCollisionPoint()) * 0.45f);
			}
		}
		else if (info.getSourceType() == TRIANGLE && info.getDestType() == ELLIPSOID)
		{
			std::cout << "triangleellipsoid!" << std::endl;
		}
	});

	// Lights
	canvas.addLight(sunLight); 
	sunLight.translate(glm::vec3(-2.0f, 0.0f, 0.5f));
	sunLight.scale(glm::vec3(0.2f, 0.2f, 0.2f));
	sunLight.setDrawing(true);
	//canvas.addModel(sunModel, false);

	//canvas.addModel(test, false);

	canvas.bloom = true;

	// Used for ray picking
	mouseRay.addColliderProperty(std::make_shared<ColliderProperties>(RayCollider({ 0,0,0 }, { 0,0,0 })));
	mouseRay.setDrawing(false);
	canvas.addModel(mouseRay, false);

	ships.emplace_back(shipSpike);
	ships.emplace_back(shipBlock);
	canvas.registerTimedCallback(spawnShip, shipSpeed, spawnIntervalms);
	shipSpike.setDrawing(false);
	shipBlock.setDrawing(false);
}

// Assigns triangle colliders based on mesh triangles
void assignTriangleColliders(Model &model)
{
	vector<Model> triangles = Triangularization::EarTriangularize(model);

	for (int i = 0; i < triangles.size(); i++)
	{
		TriangleCollider collider{	triangles[i].vertexData[0].getPoint(), 
									triangles[i].vertexData[1].getPoint(), 
									triangles[i].vertexData[2].getPoint() };
		model.addColliderProperty(std::make_shared<TriangleCollider>(collider));
	}

}

void spawnShip(int value)
{
	float spawnDistance = 5.0f;
	glm::vec3 spawnPoint = mapToHemisphere(0, ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX))) * spawnDistance;

	glm::vec3 planetWorldPos = glm::vec3(planet.getTranslation()[3]);
	//glm::mat4 rotMatrix = glm::lookAt(spawnPoint + planetWorldPos, planetWorldPos, glm::vec3{ 0,1,0 });
	//pair<float, glm::vec3> rotation = getRotationAxisAngle(rotMatrix);
	cout << ships.size() << endl;
	for (size_t i{ 0 }; i < ships.size(); i++)
	{
		if (!ships[i].drawing())
		{
			ships[i].setDrawing(true);
			ships[i].resetTransformation();
			ships[i].scale(glm::vec3(0.2f, 0.2f, 0.2f), false);
			ships[i].translate(spawnPoint + planetWorldPos);

			ships[i].addVelocity(-(spawnPoint - planetWorldPos) * (0.01f * value), true);
			break;
		}
	}

	canvas.registerTimedCallback(spawnShip, shipSpeed, spawnIntervalms);
}

//called repeatly as soon as possible
void gameLoop()
{
	sunLight.rotate(WindowCanvas::deltaCallbackTime * 28, glm::vec3(0.0f, 1.0f, 0.0f), false);
	sunLight.translate(glm::vec3(0, 0, WindowCanvas::deltaCallbackTime * 0.8f));

	// ray pick from mouse cursor
	if (leftMouseDown)
	{
		glm::vec4 screenPos{ (lastMouseX / float(WindowCanvas::windowWidth)) * 2.0f - 1.0f, 
							((lastMouseY / float(WindowCanvas::windowHeight)) * 2.0f - 1.0f) * -1.0f, // flip Y 
							-1.0f,
							1.0f };

		glm::vec4 origin =  glm::inverse(mainCamera.ProjectionMatrix * mainCamera.ViewMatrix) * screenPos;
		origin /= origin.w;
		glm::vec3 direction = glm::normalize(glm::vec3(origin) - mainCamera.getCameraPos());
		std::static_pointer_cast<RayCollider>(mouseRay.getColliderProperty(0))->setRay(origin, direction);
		// Visually represent ray picking
		mouseRay.setDrawing(true);
		mouseRay.resetTransformation();
		mouseRay.translate(glm::vec3(origin) + (direction * 10.0f));
	}
	else
	{
		mouseRay.setDrawing(false);
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
	//std::cout << x << " : " << y << std::endl;
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
