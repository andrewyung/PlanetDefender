#pragma once
#include <iostream>
#include <string>

#include <stdlib.h> 
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"
class Model;

class WindowCanvas
{
public:
	static void initializeWindow(int argc, char ** argv);
	static void start();

	static void addModel(Model &model, bool forceNewVAO);
	static void setDefaultShader(GLuint shader);

	static unsigned int assignModelID();
	~WindowCanvas();
private:
	static GLuint defaultShader;

};