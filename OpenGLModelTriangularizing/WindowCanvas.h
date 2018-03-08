#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <stdlib.h> 
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "ShaderLoader.h"
#include "Camera.h"

class Model;
class Particles;
class Light;
struct VAOInfo
{
	//maybe remove vboID as not used
	VAOInfo(GLuint vaoID, GLuint vboID, GLuint eboID, GLuint shaderID, int indexDataSize, int vertexDataSize, int nextAvailableVertexIndex) :
		vertexArrayID(vaoID), vertexBufferID(vboID), indexBufferID(eboID), shaderID(shaderID), indexDataByteSize(indexDataSize), vertexDataByteSize(vertexDataSize), nextAvailableVertexIndex(nextAvailableVertexIndex) {};

	bool drawing = true;
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint shaderID;

	bool batched = false;
	bool instanced = false;
	int instances = 1;

	//in bytes
	int vertexDataByteSize;
	int indexDataByteSize;
	int nextAvailableVertexIndex;

	glm::mat4 transformation = glm::mat4();
};

class WindowCanvas
{
public:
	static std::vector<Light*> lights;

	static void initializeWindow(int argc, char ** argv);
	static void start(void(*gameLoopCallback)(), void(*gameInitializeCallback)(), void(*mouseCallback)(int button, int state, int x, int y), void(*keyboardCallback)(unsigned char key, int x, int y));

	static void addParticles(Particles &particles, int instances, std::vector<glm::mat4> particleTransformations);
	static void addModel(Model &model, bool group);
	static void setDefaultShader(GLuint shader); 
	static void setDefaultParticleShader(GLuint shader);
	static void setCamera(Camera &camera);
	static void addLight(Light &mainLight);
	static glm::mat4 getCurrentCameraModelMatrix();

	static int frames;
	static float deltaCallbackTime;

	~WindowCanvas();
private:
	static GLuint defaultShader;
	static GLuint defaultParticleShader;
};