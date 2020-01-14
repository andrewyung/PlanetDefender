#include <iostream>

#include "GameScene.h"
#include "TextureLoader.h"

GameScene::GameScene()
{
}

void GameScene::loadTextures()
{
	greenTiledTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/GreenTiled.jpg");
	skyboxTextureAlias = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/skyboxAlias.jpg");
	earthTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/2k_earth_daymap.jpg");
	earthNormalTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/2k_earth_normal_map.jpg");
}


void GameScene::loadShaders()
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

		sunShader = ShaderLoader::load("shaders/ColorVertex.vs", "shaders/ColorFragment.fs");

		diffuseNormalShader = ShaderLoader::load("shaders/DiffuseNormalVert.vs", "shaders/DiffuseNormalFrag.fs");
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;
	}
}