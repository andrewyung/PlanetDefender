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
#include "ColliderProperties.h"

const int BLOOM_TEX_COUNT = 2;
const int PRE_PROCESS_TEX_COUNT = 2;

class Model;
class Particles;
class Light;

struct VAOInfo
{
public:
	enum Type
	{
		MODEL,
		SKYBOX,
		PARTICLE,
		POSTPROCESS
	};

	VAOInfo() = default;
	VAOInfo(GLuint vaoID, GLuint vboID, GLuint eboID, GLuint shaderID, std::vector<GLuint> textures, int indexDataSize, int vertexDataSize, int nextAvailableVertexIndex) :
		vertexArrayID(vaoID), vertexBufferID(vboID), indexBufferID(eboID), shaderID(shaderID), textures(textures), indexDataByteSize(indexDataSize), vertexDataByteSize(vertexDataSize), nextAvailableVertexIndex(nextAvailableVertexIndex) 
	{};

	bool drawing = true;
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint shaderID;
	std::vector<GLuint> textures;

	glm::vec3 velocity = glm::vec3();

	bool batched = false;
	bool instanced = false;
	bool depthMask = true;
	Type renderType = MODEL;
	int instances = 1;

	std::vector<std::shared_ptr<ColliderProperties>> colliderProp;

	//in bytes
	int vertexDataByteSize;
	int indexDataByteSize;
	// Used to keep track and group static VAOs 
	int nextAvailableVertexIndex;

	glm::mat4 rotation;
	glm::mat4 scale;
	glm::mat4 translation;
};

class WindowCanvas
{
public:
	WindowCanvas();

	std::vector<Light*> lights;

	void initializeWindow(int argc, char ** argv);
	void start(void(*gameLoopCallback)(), void(*gameInitializeCallback)(), void(*mouseCallback)(int button, int state, int x, int y), void(*keyboardCallback)(unsigned char key, int x, int y), void(*mouseMotionCallback)(int x, int y));

	void addParticles(Particles &particles, int instances, std::vector<glm::mat4> particleTransformations);
	void addModel(Model &model, bool group, VAOInfo::Type renderType = VAOInfo::Type::MODEL, bool depthMask = true);
	void addSkybox(GLuint skyboxTextures);

	static void setDefaultShader(GLuint shader); 
	static void setDefaultParticleShader(GLuint shader);
	void setCamera(Camera &camera);
	void addLight(Light &mainLight);
	static glm::mat4 getCurrentCameraModelMatrix();

	void createPostprocessFrameBuffer();
	void WindowCanvas::createBloomFrameBuffer();

	static void renderCall();

	GLuint preprocessFBO;
	GLuint preprocessTextures[PRE_PROCESS_TEX_COUNT];
	GLuint preprocessBuffersAttachment[PRE_PROCESS_TEX_COUNT];

	void drawPostprocessQuad(GLuint texture, ...);

	static int frames;
	static float deltaCallbackTime;

	GLuint bloomTexture[2];
	GLuint bloomFBO[2];
	bool bloom;

	std::unique_ptr<Model> postprocessingQuad;

	static const int windowHeight = 720;
	static const int windowWidth = 1280;

private:
	static GLuint defaultShader;
	static GLuint defaultParticleShader;

	void applyBloom();
	void renderScene();

	static WindowCanvas* instance;

	GLuint preprocessShader, postprocessShader, bloomShader;
};