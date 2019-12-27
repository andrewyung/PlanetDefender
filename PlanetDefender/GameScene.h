#pragma once

#include "Model.h"
#include "Camera.h"

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

	GLuint diffuseShader;
	GLuint greenTiledTexture, skyboxTextureAlias;

	Model* planet;

	Model* test;

	Camera mainCamera;

	void loadTextures();
	void loadShaders();
};

