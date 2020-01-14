#pragma once

#include "Model.h"
#include "Camera.h"
#include "Light.h"

#include <vector>

struct Texture
{
	GLuint textureID;
	char* textureName;
};

struct Shader
{
	GLuint shaderID;
	char* shaderName;
};

class GameScene
{
public:
	GameScene();

	std::vector<Texture> sceneTextures;
	std::vector<Shader> sceneShaders;
	std::vector<Model> sceneModels;

	GLuint diffuseShader, sunShader;
	GLuint greenTiledTexture, skyboxTextureAlias;

	Model* planet;

	Model* test;

	Model* sunModel;
	Light* sunLight;

	Camera mainCamera;

	void loadTextures();
	void loadShaders();
};

