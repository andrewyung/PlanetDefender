#include <iostream>

#include "GameScene.h"
#include "TextureLoader.h"

GameScene::GameScene()
{
}

void GameScene::loadAll()
{
	loadTextures();
	loadShaders();
}

void GameScene::loadTextures()
{
	greenTiledTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/GreenTiled.jpg");
	skyboxTextureAlias = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/skyboxAlias.jpg");
	earthTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/earth.png");
	
	// Is not for planet and low res but more noticeable change in normals
	//earthNormalTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/normal_mapping_normal_map.png");
	earthNormalTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/earth_normalmap.png");
	earthSpecularTexture = TextureLoader::load("C:/Users/Andrew/Documents/GitHub/PlanetDefender/PlanetDefender/Textures/2k_earth_specular_map.png");
}


void GameScene::loadShaders()
{
	//load shaders
	try
	{
		GLuint shaderID = ShaderLoader::load("shaders/DefaultVertex.vs", "shaders/DefaultFragment.fs");
		std::cout << "shader " << shaderID << std::endl;
		//set a shader for models to use if not set
		WindowCanvas::setDefaultShader(shaderID);

		GLuint particleShaderID = ShaderLoader::load("shaders/DefaultParticleVertex.vs", "shaders/DefaultFragment.fs");
		std::cout << "shader " << particleShaderID << std::endl;
		WindowCanvas::setDefaultParticleShader(particleShaderID);

		diffuseShader = ShaderLoader::load("shaders/DiffuseTextureVert.vs", "shaders/DiffuseTextureFrag.fs");
		std::cout << "shader " << diffuseShader << std::endl;

		sunShader = ShaderLoader::load("shaders/ColorVertex.vs", "shaders/ColorFragment.fs");
		std::cout << "shader " << sunShader << std::endl;

		diffuseNormalShader = ShaderLoader::load("shaders/DiffuseNormalVert.vs", "shaders/DiffuseNormalFrag.fs");
		std::cout << "shader " << diffuseNormalShader << std::endl;

		lightShader = ShaderLoader::load("shaders/LightVert.vs", "shaders/LightFrag.fs");
		std::cout << "shader " << lightShader << std::endl;

		shipShader = ShaderLoader::load("shaders/ShipVert.vs", "shaders/ShipFrag.fs");
		std::cout << "shader " << shipShader << std::endl;
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Could not set default shader : " << e.what() << std::endl;
	}
}