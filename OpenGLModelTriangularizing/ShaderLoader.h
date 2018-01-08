#pragma once
#include <GL\glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderLoader
{
public:
	static GLuint load(const GLchar* vertexString, const GLchar* fragmentString);

	static void setFloat(GLuint shaderID, const std::string &name, float value);
	static void setMat4x4(GLuint shaderID, const std::string &name, glm::mat4 matrix4x4);
};

