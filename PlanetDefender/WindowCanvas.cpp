#pragma once

#include "FileOperations.h"
#include "WindowCanvas.h"
#include "Model.h"
#include "Particles.h"
#include "Light.h"
#include "ModelLoader.h"
#include "EllipsoidCollider.h"

WindowCanvas* WindowCanvas::instance;

GLuint WindowCanvas::defaultShader;
GLuint WindowCanvas::defaultParticleShader;
Camera *camera;

const int TARGET_FPS = 60;
const int DEFAULT_BUFFER_SIZE = 16;

std::vector<std::shared_ptr<VAOInfo>> vertexArrayIDs;
int WindowCanvas::frames = 0;
float WindowCanvas::deltaCallbackTime = 0;
ShaderLoader shaderLoader;
int lastCallbackTime;

void(*externalGameLoop)();

void glErrorCheck();

void checkFramebuffer()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

//used for debugging buffers
void printVertexBufferContent(GLuint bufferID)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const int FLOATS_TO_READ = 100;
	float result[FLOATS_TO_READ];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, FLOATS_TO_READ  * sizeof(float), result);

	for (int i = 0; i < FLOATS_TO_READ; i++)
	{
		printf("%f \n", result[i]);
	}
}

void WindowCanvas::applyBloom()
{
	/*
	postprocessingQuad->shader = bloomShader;
	bool horizontal = true, first_iteration = true;
	int amount = 0;
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO[horizontal]);
		ShaderLoader::setInt(postprocessingQuad->shader, "horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? preprocessTextures[1] : bloomTexture
		);

		drawPostprocessQuad();
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}*/
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	postprocessingQuad->shader = postprocessShader;
	drawPostprocessQuad();
}

void WindowCanvas::drawPostprocessQuad()
{
	glClearColor(0.0, 0.0, 0.0, 1);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	std::shared_ptr<VAOInfo> currentVAO = postprocessingQuad->vaoInfo;

	glUseProgram(currentVAO->shaderID);

	shaderLoader.setMat4x4(currentVAO->shaderID, "transform", (currentVAO->translation * currentVAO->rotation * currentVAO->scale));
	/*
	if (bloom)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, bloomTexture);
	}*/
	for (int i{ 0 }; i < PRE_PROCESS_TEX_COUNT; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		glBindTexture(GL_TEXTURE_2D, preprocessTextures[i]);
	}
	glEnable(GL_TEXTURE_2D);

	glBindVertexArray(currentVAO->vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentVAO->indexBufferID);

	glDrawElements(GL_TRIANGLES, currentVAO->indexDataByteSize / sizeof(int), GL_UNSIGNED_INT, 0);
}

void WindowCanvas::renderCall()
{
	instance->renderScene();
}

//render call
void WindowCanvas::renderScene() 
{
	if (bloom)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, preprocessFBO);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//better way of doing this rather than getting all positions every render frame
	std::vector <glm::vec4> lightPositions;
	std::vector <glm::vec3> lightColors;
	for (int i = 0; i < lights.size(); i++)
	{
		lightPositions.push_back(lights[i]->getLightPosition());
		lightColors.push_back(lights[i]->lightColor);
	}

	GLint currentShader{ 0 };
	//go through all VAOs that need to be rendered
	for (int i = 0; i < vertexArrayIDs.size(); i++)
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader);

		std::shared_ptr<VAOInfo> currentVAO = vertexArrayIDs[i];
		// Skip post processing models
		if (currentVAO->depthMask)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if (currentVAO->drawing)//is vao is set to be rendered
		{
			//if current shader isn't the one vao is to use
			if (currentShader != currentVAO->shaderID)
			{
				glUseProgram(currentVAO->shaderID);
			}

			//MVP temp for vertex shaders that were used for testing
			shaderLoader.setMat4x4(currentVAO->shaderID, "MVP", camera->getMVP());
			shaderLoader.setMat4x4(currentVAO->shaderID, "model", camera->ModelMatrix);
			if (currentVAO->renderType == VAOInfo::Type::SKYBOX)
			{
				// Removes translation of transformation
				shaderLoader.setMat4x4(currentVAO->shaderID, "view", glm::mat4(glm::mat3(camera->ViewMatrix)));
			}
			else
			{
				shaderLoader.setMat4x4(currentVAO->shaderID, "view", camera->ViewMatrix);
			}
			shaderLoader.setMat4x4(currentVAO->shaderID, "projection", camera->ProjectionMatrix);
			
			if (lightPositions.size() > 0)
			{
				shaderLoader.setVector4(currentVAO->shaderID, "lightPos", lightPositions.size(), glm::value_ptr(lightPositions[0]));
				shaderLoader.setVector3(currentVAO->shaderID, "lightColor", lightPositions.size(), glm::value_ptr(lightColors[0]));
			}

			shaderLoader.setMat4x4(currentVAO->shaderID, "transform", (currentVAO->translation * currentVAO->rotation * currentVAO->scale));
			shaderLoader.setInt(currentVAO->shaderID, "time", glutGet(GLUT_ELAPSED_TIME));

			// Textures
			if (currentVAO->textures.size() != 0)
			{
				for (int i = 0; i < currentVAO->textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, currentVAO->textures[i]);
				}
			}
			glEnable(GL_TEXTURE_2D);

			//std::cout << time << std::endl;
			glBindVertexArray(currentVAO->vertexArrayID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentVAO->indexBufferID);
			//std::cout << "running - model added : vb" << currentVAO.vertexDataByteSize << " : ib" << currentVAO.indexDataByteSize << std::endl;


			if (currentVAO->instanced)//this vao is to be instanced
			{
				glDrawElementsInstanced(GL_TRIANGLES, currentVAO->indexDataByteSize / sizeof(int), GL_UNSIGNED_INT, 0, currentVAO->instances);
			}
			else
			{
				glDrawElements(GL_TRIANGLES, currentVAO->indexDataByteSize / sizeof(int), GL_UNSIGNED_INT, 0);
			}
		}
	}

	if (bloom)
	{
		glDrawBuffers(PRE_PROCESS_TEX_COUNT, preprocessBuffersAttachment);

		applyBloom();
	}

	glutSwapBuffers();

	//clear vao binds
	glBindVertexArray(0);
}

void WindowCanvas::createBloomFrameBuffer()
{
	glGenFramebuffers(2, bloomFBO);

	glGenTextures(BLOOM_TEX_COUNT, &bloomTexture);

	glBindTexture(GL_TEXTURE_2D, bloomTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i{ 0 }; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO[i]);

		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTexture, 0
		);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WindowCanvas::createPostprocessFrameBuffer()
{
	// Pre process FBO
	glGenFramebuffers(1, &preprocessFBO); // FBO for rendering all objects on

	glBindFramebuffer(GL_FRAMEBUFFER, preprocessFBO);

	glGenTextures(PRE_PROCESS_TEX_COUNT, preprocessTextures);

	for (unsigned int i{ 0 }; i < PRE_PROCESS_TEX_COUNT; i++)
	{
		glBindTexture(GL_TEXTURE_2D, preprocessTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, preprocessTextures[i], 0
		);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i{ 0 }; i < PRE_PROCESS_TEX_COUNT; i++)
	{
		preprocessBuffersAttachment[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	checkFramebuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	preprocessShader = ShaderLoader::load("shaders/PreprocessVert.vs", "shaders/PreprocessFrag.fs");
	postprocessShader = ShaderLoader::load("shaders/PostprocessVert.vs", "shaders/PostprocessFrag.fs");
	bloomShader = ShaderLoader::load("shaders/BloomVert.vs", "shaders/BloomFrag.fs");

	postprocessingQuad = std::make_unique<Model>(ModelLoader::createPrimitive(ModelLoader::QUAD));
	postprocessingQuad->shader = preprocessShader;
	addModel(*postprocessingQuad, false);
	postprocessingQuad->rotate(-90, glm::vec3(0, 0, 1));
	postprocessingQuad->rotate(180, glm::vec3(0, 1, 0));
	postprocessingQuad->setDrawing(false);
	postprocessingQuad->getVAOInfo()->renderType = VAOInfo::POSTPROCESS;

	createBloomFrameBuffer();
}

void glErrorCheck()
{
	GLenum err;
	while ((err = glGetError())) {
		std::cout << err;
	}
}

//timer that queues for rendering at TARGET_FPS frames per second
void frameTimer(int value)
{
	glutTimerFunc(1000 / TARGET_FPS, frameTimer, ++value);

	WindowCanvas::frames++;

	glutPostRedisplay();
	//std::cout << WindowCanvas::deltaFrameTime << std::endl;
}

WindowCanvas::WindowCanvas()
{
	instance = this;
}

//initalizes glut
void WindowCanvas::initializeWindow(int argc, char **argv)
{
	//create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition((GLUT_SCREEN_WIDTH / 2) - (windowWidth / 2), (GLUT_SCREEN_HEIGHT / 2) - (windowHeight / 2));
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("Window");

	glewInit();

	createPostprocessFrameBuffer();
}

// AABB collision with spheres and rays. This only checks models that has had transformation modified in the last frame (indicated by transformUpdated in VAOInfo)
void collisionCheck()
{
	// For each VAO
	for (int i = 0; i < vertexArrayIDs.size(); i++)
	{
		std::shared_ptr<VAOInfo> vaoInfo = vertexArrayIDs[i];
		// Has velocity and collider
		if (vaoInfo->velocity.length != 0 && vaoInfo->colliderProp.size() > 0)
		{
			// For each collider property
			for (int colliderPropIndex = 0; colliderPropIndex < vaoInfo->colliderProp.size(); colliderPropIndex++)
			{
				if (auto properties = std::dynamic_pointer_cast<EllipsoidCollider>(vaoInfo->colliderProp[colliderPropIndex]))
				{
					glm::mat4 transformation = vaoInfo->translation * vaoInfo->rotation * vaoInfo->scale * glm::scale(glm::mat4(), properties->getDimensions()); // include dimension!!!!
					glm::mat4 ellipsoidSpace = glm::inverse(transformation);
					

					// With ellipsoidSpace the ellipsoid is a unit circle at (0, 0, 0)
					//glm::distance()
				}
			}

			// When no collision then move as expected
			vaoInfo->translation = glm::translate(vaoInfo->translation, vaoInfo->velocity * WindowCanvas::deltaCallbackTime);
		}
	}
}

//wraps the game loop with anything that needs to be done before and/or after
void gameLoopWrapper()
{
	WindowCanvas::deltaCallbackTime = (glutGet(GLUT_ELAPSED_TIME) - lastCallbackTime) * 0.001f;//

	collisionCheck();
	
	if (WindowCanvas::deltaCallbackTime != 0)//can be increased to control frequency of external gmae loop call
	{
		lastCallbackTime = glutGet(GLUT_ELAPSED_TIME);

		externalGameLoop();
	}
}

//register callbacks and starts glut main loop
void WindowCanvas::start(void (*gameLoopCallback)(), 
						void(*gameInitializeCallback)(), 
						void(*mouseCallback)(int button, int state, int x, int y), 
						void(*keyboardCallback)(unsigned char key, int x, int y),
						void(*mouseMotionCallback)(int x, int y))
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE); 
	//glEnable(GL_FRAMEBUFFER_SRGB); //Gamma color

	gameInitializeCallback();
	externalGameLoop = gameLoopCallback;

	// register callbacks
	glutTimerFunc(0, frameTimer, 0);
	glutIdleFunc(gameLoopWrapper);
	glutDisplayFunc(renderCall);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);

	glutMainLoop();
}

//resize the param id buffer
void resizeBufferObject(GLenum type, GLuint id, int currentSize, int toSize, GLenum usage)
{
	// does 2 copies so vertex attributes don't have to be redefined if using a new buffer and type is vbo

	std::cout << "resizing buffer" << std::endl;
	GLuint tempBufferObject;

	glBindBuffer(type, id);

	glGenBuffers(1, &tempBufferObject);
	glBindBuffer(GL_COPY_WRITE_BUFFER, tempBufferObject);
	glBufferData(GL_COPY_WRITE_BUFFER, currentSize, NULL, GL_STATIC_COPY);

	//copy currently bound buffer object to tempBufferObject
	glCopyBufferSubData(type, GL_COPY_WRITE_BUFFER, 0, 0, currentSize);

	//recreate buffer object
	glBufferData(type, toSize, NULL, usage);

	//copy back from tempBufferObject to buffer object
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, type, 0, 0, currentSize);

	glDeleteBuffers(1, &tempBufferObject);

	std::cout << "resized from " << currentSize << " to " << toSize << std::endl;
}

void WindowCanvas::addParticles(Particles &particles, int instances, std::vector<glm::mat4> particleTransformations)
{
	if (particleTransformations.empty())
	{
		particleTransformations.resize(instances);
	}
	if (particleTransformations.size() != instances)
	{
		std::cout << "number of particle transformations (" << particleTransformations.size() << ") isnt equal to instances (" << instances << ")" << std::endl;
		return;
	}

	if (particles.shader == 0)
	{
		if (defaultParticleShader == 0)
		{
			std::cout << "Default particle shader not set" << std::endl;
		}
		particles.shader = defaultParticleShader;
	}

	//array for the raw vertex data thats to be put into the vbo
	std::vector<float> vertexArrayData;

	//vertex information from model
	for (int i = 0; i < particles.particleModel->vertexData.size(); i++)
	{
		Vertex currentVertex = particles.particleModel->vertexData[i];

		//position
		vertexArrayData.push_back(currentVertex.x);
		vertexArrayData.push_back(currentVertex.y);
		vertexArrayData.push_back(currentVertex.z);

		//rgba
		vertexArrayData.push_back(currentVertex.r);
		vertexArrayData.push_back(currentVertex.g);
		vertexArrayData.push_back(currentVertex.b);
		vertexArrayData.push_back(currentVertex.a);

		//texture uv
		vertexArrayData.push_back(currentVertex.xUV);
		vertexArrayData.push_back(currentVertex.yUV);

		//normal
		vertexArrayData.push_back(currentVertex.xNormal);
		vertexArrayData.push_back(currentVertex.yNormal);
		vertexArrayData.push_back(currentVertex.zNormal);

		//tangent
		vertexArrayData.push_back(currentVertex.xTangent);
		vertexArrayData.push_back(currentVertex.yTangent);
		vertexArrayData.push_back(currentVertex.zTangent);
	}


	std::vector<float> particleTransformationsData;

	for (int i = 0; i < instances; i++)
	{
		const float *matArray = (const float*)glm::value_ptr(particleTransformations[i]);
		//particle transformation
		for (int i = 0; i < 16; i++)//4x4 matrix, 16 values
		{
			particleTransformationsData.push_back(matArray[i]);
		}
	}


	//no vao with matching shader found, create a new vao
	GLuint VAO, VBO, transformVBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &transformVBO);

	std::shared_ptr<VAOInfo> info = std::make_shared<VAOInfo>(VAO, VBO, EBO,											//id's
															particles.shader,											//shader program
															particles.textures,											//textures
															particles.particleModel->indexData.size() * sizeof(int),	//index size of particle model
															vertexArrayData.size() * sizeof(float),						//vertex size
															particles.particleModel->vertexData.size());				//number of vertices of particle model
	info->instanced = true;
	info->instances = instances;
	info->renderType = VAOInfo::Type::PARTICLE;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayData.size() * sizeof(float), &vertexArrayData[0], GL_STATIC_DRAW);

	//interpretation of data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, 15 * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
	
	//vbo containing particle transformations
	glBindBuffer(GL_ARRAY_BUFFER, transformVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &particleTransformationsData[0], GL_STREAM_DRAW);

	// Transformation is particle input
	int pos = glGetAttribLocation(particles.shader, "particleTransformations");
	std::cout << "should be 4 attributes... " << pos << std::endl;
	int pos1 = pos + 0;
	int pos2 = pos + 1;
	int pos3 = pos + 2;
	int pos4 = pos + 3;
	glEnableVertexAttribArray(pos1);
	glEnableVertexAttribArray(pos2);
	glEnableVertexAttribArray(pos3);
	glEnableVertexAttribArray(pos4);
	glVertexAttribPointer(pos1, 5, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
	glVertexAttribPointer(pos2, 5, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
	glVertexAttribPointer(pos3, 5, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
	glVertexAttribPointer(pos4, 5, GL_FLOAT, GL_TRUE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);

	//element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles.particleModel->indexData.size() * sizeof(float), &particles.particleModel->indexData[0], GL_STATIC_DRAW);

	//this model uses the vao that was pushed into the vao list right above
	particles.setVAOInfo(*info);

	vertexArrayIDs.push_back(info);

	glBindVertexArray(0);
	std::cout << "particle created" << particles.shader << std::endl;
}

//Called externally to add models to be rendered. 
//Allows for grouping of models so that multiple objects can be done with a single draw (such as static models that share the same shader)
//TODO: instancing
void WindowCanvas::addModel(Model &model, bool group, VAOInfo::Type renderType, bool depthMask)
{
	if (model.shader == 0)
	{
		if (defaultShader == 0)
		{
			std::cout << "Default shader not set" << std::endl;
		}
		model.shader = defaultShader;
	}

	//array for the raw vertex data thats to be put into the vbo
	std::vector<float> vertexArrayData;
	//vertex information from model
	for (int i = 0; i < model.vertexData.size(); i++)
	{
		Vertex currentVertex = model.vertexData[i];

		//position
		vertexArrayData.push_back(currentVertex.x);
		vertexArrayData.push_back(currentVertex.y);
		vertexArrayData.push_back(currentVertex.z);

		//rgba
		vertexArrayData.push_back(currentVertex.r);
		vertexArrayData.push_back(currentVertex.g);
		vertexArrayData.push_back(currentVertex.b);
		vertexArrayData.push_back(currentVertex.a);

		//texture uv
		vertexArrayData.push_back(currentVertex.xUV);
		vertexArrayData.push_back(currentVertex.yUV);

		//normal
		vertexArrayData.push_back(currentVertex.xNormal);
		vertexArrayData.push_back(currentVertex.yNormal);
		vertexArrayData.push_back(currentVertex.zNormal);

		//tangent
		vertexArrayData.push_back(currentVertex.xTangent);
		vertexArrayData.push_back(currentVertex.yTangent);
		vertexArrayData.push_back(currentVertex.zTangent);
	}

	if (group)
	{
		for (int i = 0; i < vertexArrayIDs.size(); i++)
		{
			//uses same shader, include in existing vao
			if (model.shader == vertexArrayIDs[i]->shaderID && !vertexArrayIDs[i]->instanced && vertexArrayIDs[i]->batched)
			{
				std::shared_ptr<VAOInfo> currentVAOInfo = vertexArrayIDs[i];
				currentVAOInfo->renderType = renderType;
				currentVAOInfo->depthMask = depthMask;
				currentVAOInfo->textures = model.textures;

				currentVAOInfo->colliderProp.insert(currentVAOInfo->colliderProp.end(), model.colliderProperties.begin(), model.colliderProperties.end());

				glBindVertexArray(currentVAOInfo->vertexArrayID);

				int currentBufferSize;
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &currentBufferSize);
				//used to multiply the current size if there is more data than buffer can currently hold
				int ceilSizeRatio = ceil((currentVAOInfo->vertexDataByteSize + (vertexArrayData.size() * sizeof(float))) / (float) currentBufferSize);
				if (ceilSizeRatio > 1)
				{
					resizeBufferObject(GL_ARRAY_BUFFER, currentVAOInfo->vertexArrayID, currentBufferSize, currentBufferSize * ceilSizeRatio, GL_DYNAMIC_DRAW);
				}

				glBindBuffer(GL_ARRAY_BUFFER, currentVAOInfo->vertexBufferID);
				glBufferSubData(GL_ARRAY_BUFFER, currentVAOInfo->vertexDataByteSize, vertexArrayData.size() * sizeof(float), &vertexArrayData[0]);
				
				//std::cout << currentVAOInfo.vertexBufferID << std::endl;

				//set indices to proper index offset
				for (int i = 0; i < model.indexData.size(); i++)
				{
					model.indexData[i] += currentVAOInfo->nextAvailableVertexIndex;
				}

				glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &currentBufferSize);
				//used to multiply the current size if there is more data than buffer can currently hold
				ceilSizeRatio = ceil((currentVAOInfo->indexDataByteSize + (model.indexData.size() * sizeof(int))) / (float) currentBufferSize);
				if (ceilSizeRatio > 1)
				{
					resizeBufferObject(GL_ELEMENT_ARRAY_BUFFER, currentVAOInfo->indexBufferID, currentBufferSize, currentBufferSize * ceilSizeRatio, GL_DYNAMIC_DRAW);
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentVAOInfo->indexBufferID);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentVAOInfo->indexDataByteSize, model.indexData.size() * sizeof(int), &model.indexData[0]);
				//std::cout << currentVAOInfo.indexBufferID << std::endl;

				//update where the data for model begins in the buffers
				model.setVertexBufferAndArrayData(currentVAOInfo->vertexDataByteSize, currentVAOInfo->indexDataByteSize);
				model.setVAOInfo(currentVAOInfo);

				//update size of buffers
				currentVAOInfo->vertexDataByteSize += vertexArrayData.size() * sizeof(float);
				currentVAOInfo->indexDataByteSize += model.indexData.size() * sizeof(int);
				currentVAOInfo->nextAvailableVertexIndex += model.vertexData.size();


				//printVertexBufferContent(currentVAOInfo->vertexBufferID);
				//printIndexBufferContent(currentVAOInfo.indexBufferID);

				glBindVertexArray(0);
				std::cout << "model added : vb " << currentVAOInfo->vertexDataByteSize << " : ib "  << currentVAOInfo->indexDataByteSize << std::endl;

				return;
			}
		}
	}
	//no vao with matching shader found, create a new vao
	GLuint VAO, VBO, EBO;	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::shared_ptr<VAOInfo> info = std::make_shared<VAOInfo>(VAO, VBO, EBO,								//id's
																model.shader,								//shader program
																model.textures,								//textures
																model.indexData.size() * sizeof(int),		//index size
																vertexArrayData.size() * sizeof(float),		//vertex size
																model.vertexData.size());					//number of vertices
	info->batched = group;
	info->renderType = renderType;
	info->depthMask = depthMask;

	info->colliderProp.insert(info->colliderProp.end(), model.colliderProperties.begin(), model.colliderProperties.end());

	glBindVertexArray(VAO);

	// > 1 when default buffer size isn't large enough
	int bufferSizeMultiplier = ceil((vertexArrayData.size() * sizeof(float)) / (float) DEFAULT_BUFFER_SIZE);

	//std::cout << bufferSizeMultiplier << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, DEFAULT_BUFFER_SIZE * bufferSizeMultiplier, &vertexArrayData[0], GL_STATIC_DRAW);

	//interpretation of data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, 15 * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);

	bufferSizeMultiplier = ceil((model.indexData.size() * sizeof(int)) / (float) DEFAULT_BUFFER_SIZE);

	//element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DEFAULT_BUFFER_SIZE * bufferSizeMultiplier, &model.indexData[0], GL_STATIC_DRAW);

	//this model uses the vao that was pushed into the vao list right above
	model.setVAOInfo(info);
	//only object in buffer, so no offset
	model.setVertexBufferAndArrayData(0, 0);

	vertexArrayIDs.push_back(info);

	glBindVertexArray(0);
	std::cout << "model created" << std::endl;
}

void WindowCanvas::addSkybox(GLuint skyboxTextures)
{
	FileOperations fileOp;

	GLuint skyboxShader = ShaderLoader::load("shaders/SkyboxVert.vs", "shaders/SkyboxFrag.fs");

	Model skybox = ModelLoader::createPrimitive(ModelLoader::SKYBOX_CUBE);
	skybox.shader = skyboxShader;
	skybox.textures.push_back(skyboxTextures);

	addModel(skybox, false, VAOInfo::Type::SKYBOX, false);
}

void WindowCanvas::setDefaultShader(GLuint shader)
{
	defaultShader = shader;
}

void WindowCanvas::setDefaultParticleShader(GLuint shader)
{
	defaultParticleShader = shader;
}

void WindowCanvas::setCamera(Camera &mainCamera)
{
	camera = &mainCamera;
}

void WindowCanvas::addLight(Light &light)
{
	WindowCanvas::lights.push_back(&light);
	addModel(light, false);
}

glm::mat4 WindowCanvas::getCurrentCameraModelMatrix()
{
	if (camera == nullptr)
	{
		return glm::mat4();
	}
	return camera->ModelMatrix;
}

