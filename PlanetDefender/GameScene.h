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

	GLuint diffuseShader, sunShader, diffuseNormalShader, lightShader;
	GLuint greenTiledTexture, skyboxTextureAlias;
	GLuint earthTexture, earthNormalTexture, earthSpecularTexture;


	std::vector<Texture> sceneTextures;
	std::vector<Shader> sceneShaders;
	std::vector<Model> sceneModels;

	void loadAll();
	void loadTextures();
	void loadShaders();
};

