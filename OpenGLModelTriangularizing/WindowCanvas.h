#pragma once
#include <iostream>
#include <string>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"
#include "Model.h"

class WindowCanvas
{
public:
	static void initializeWindow(int argc, char ** argv);
	static void start();

	static void addModel(Model &model);
	static void setDefaultShader(GLuint shader);

private:
	static GLuint defaultShader;
};