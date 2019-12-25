#pragma once

#include <GL\glew.h>
#include <vector>

class TextureLoader
{
public:
	static GLuint load(char* textureFilename);
	static GLuint loadCubemap(std::vector<std::string> faces);
		
};

