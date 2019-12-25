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
	static GLuint load(char* vertexFilepath, char* fragmentFilepath);

	static void setInt(GLuint shaderID, const std::string &name, int value);
	static void setFloat(GLuint shaderID, const std::string &name, float value);
	static void setMat4x4(GLuint shaderID, const std::string &name, glm::mat4 matrix4x4);
	static void setVector3(GLuint shaderID, const std::string & name, glm::vec3 vector);
	static void setVector3(GLuint shaderID, const std::string & name, int numberOfVectors, float * vectors);
	static void setVector4(GLuint shaderID, const std::string & name, glm::vec4 vector);
	static void setVector4(GLuint shaderID, const std::string & name, int numberOfVectors, float * vectors);
};

