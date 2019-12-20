#include <iostream>

#include "TextureLoader.h"
#include "stb_image.h"


GLuint TextureLoader::load(char * textureFilename)
{
	unsigned int texture = 0;

	int width, height, nrChannels;
	unsigned char *data = stbi_load(textureFilename, &width, &height, &nrChannels, 0);

	if (data)
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << textureFilename << " loaded" << std::endl;
	}
	else
	{
		std::cout << "Could not load texture " << textureFilename << std::endl;
	}

	stbi_image_free(data);

	return texture;
}
