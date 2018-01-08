#include "ShaderLoader.h"

GLuint ShaderLoader::load(const GLchar* vertexString, const GLchar* fragmentString)
{
	GLuint vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	//assign vertexShader from vertexString
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexString, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	
		glDeleteShader(vertexShader);

		std::string errorMessage = "Shader vertexString compilation error\n"; 
		errorMessage += infoLog;
		throw std::invalid_argument(errorMessage);
	};

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentString, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::string errorMessage = "Shader fragmentString compilation error\n";
		errorMessage += infoLog;
		throw std::invalid_argument(errorMessage);
	}

	GLuint shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	// print linking errors if any
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (!success)
	{
		glGetProgramInfoLog(shader, 512, NULL, infoLog);

		std::string errorMessage = "Shader linking error\n";
		errorMessage += infoLog;
		throw std::invalid_argument(errorMessage);
	}

	glUseProgram(shader);
	//initialize the 4x4 transform matrix in shader to identity
	setMat4x4(shader, "transform", glm::mat4());

	return shader;
}

void ShaderLoader::setFloat(GLuint shaderID, const std::string &name, float value)
{
	glUniform1d(glGetUniformLocation(shaderID, name.c_str()), value);
}

void ShaderLoader::setMat4x4(GLuint shaderID, const std::string & name, glm::mat4 matrix4x4)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix4x4));
}



