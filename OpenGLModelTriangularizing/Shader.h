#pragma once
#include <GL\glew.h>
#include <string>
static class ShaderLoader
{
public:
	GLuint load(char* vertexString, char* fragmentString);
private:
	std::string defaultVertexFilePath = "hi", 
				defaultFragmentFilePath = "ho";
};

