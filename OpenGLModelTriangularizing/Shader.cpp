#include "Shader.h"
#include <iostream>

GLuint ShaderLoader::load(char* vertexString, char* fragmentString)
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

		glDeleteShader(fragmentShader);

		std::string errorMessage = "Shader vertexString compilation error\n";
		errorMessage += infoLog;
		throw std::invalid_argument(errorMessage);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	// print linking errors if any
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 512, NULL, infoLog);

		std::string errorMessage = "Shader linking error\n";
		errorMessage += infoLog;
		throw std::invalid_argument(errorMessage);
	}

	return shader;
}
