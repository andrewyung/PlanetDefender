#pragma once
#include <string>
#include "Shader.h"

class Window
{
public:
	void createWindow(int argc, char ** argv);
	void redraw();

	void addShader(Object &targetObject, std::string vertexFilePath, std::string fragmentFilePath);
	void addDefaultShader(Object &targetObject);
private:
};