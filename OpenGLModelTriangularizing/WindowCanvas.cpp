#include "WindowCanvas.h"
#include "Model.h"
#define BUFFER_OFFSET(offset) (static_cast<char*>(0) + (offset))

GLuint WindowCanvas::defaultShader;

const int DEFAULT_BUFFER_SIZE = 16;

struct VAOInfo
{
	VAOInfo(GLuint vaoID, GLuint vboID, GLuint eboID, GLuint shaderID, int indexDataSize, int vertexDataSize, int nextAvailableVertexIndex) :
		vertexArrayID(vaoID), vertexBufferID(vboID), indexBufferID(eboID), shaderID(shaderID), indexDataByteSize(indexDataSize), vertexDataByteSize(vertexDataSize), nextAvailableVertexIndex(nextAvailableVertexIndex){};

	bool instanced = false;
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint shaderID;

	//in bytes
	int vertexDataByteSize;
	int indexDataByteSize;
	int nextAvailableVertexIndex;
};

std::vector<VAOInfo> vertexArrayIDs;
ShaderLoader shaderLoader;

int frames = 0;


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
void printIndexBufferContent(GLuint bufferID)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	const int INTS_TO_READ = 100;
	int result[INTS_TO_READ];
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, INTS_TO_READ  * sizeof(int), result);

	for (int i = 0; i < INTS_TO_READ; i++)
	{
		printf("%i \n", (int) result[i]);
	}
}

void renderScene(void) {
	int time = glutGet(GLUT_ELAPSED_TIME);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1);

	GLint currentShader;
	//go through all VAOs that need to be rendered
	for (int i = 0; i < vertexArrayIDs.size(); i++)
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader);

		VAOInfo currentVAO = vertexArrayIDs[i];
		//if current shader isn't the one vao is to use
		if (currentShader != currentVAO.shaderID)
		{
			glUseProgram(currentVAO.shaderID);
		}
		//move shader uniform changes to game loop later
		if (i == 0)
		{
			glm::mat4 transform;
			transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
			transform = glm::translate(transform, glm::vec3(0.05f, -0.05f, 0.0f) * ((float)(frames % 100)));
			transform = glm::rotate_slow(transform, glm::radians(1.0f) * frames, glm::vec3(1.0f, 0.0f, 1.0f));
			shaderLoader.setMat4x4(currentVAO.shaderID, "transform", transform);
		}
		else if (i == 1)
		{
			glm::mat4 transform;
			transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
			transform = glm::rotate(transform, glm::radians(50.0f), glm::vec3(1, 1, 1));
			shaderLoader.setMat4x4(currentVAO.shaderID, "transform", transform);
		}
		else if (i == 2)
		{
			glm::mat4 transform;
			transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
			transform = glm::rotate(transform, glm::radians(50.0f), glm::vec3(1, 1, 1));
			shaderLoader.setMat4x4(currentVAO.shaderID, "transform", transform);
		}
		shaderLoader.setInt(currentVAO.shaderID, "time", time);
		//std::cout << time << std::endl;
		glBindVertexArray(currentVAO.vertexArrayID);	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentVAO.indexBufferID);
		//std::cout << "running - model added : vb" << currentVAO.vertexDataByteSize << " : ib" << currentVAO.indexDataByteSize << std::endl;

		glDrawElements(GL_TRIANGLES, currentVAO.indexDataByteSize / sizeof(int), GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();

	//clear vao binds
	glBindVertexArray(0);
}

void frameTimer(int value)
{
	const int targetFPS = 60;
	glutTimerFunc(1000 / targetFPS, frameTimer, ++value);

	frames++;
	//std::cout << "frame" << std::endl;
	glutPostRedisplay();
}

void WindowCanvas::initializeWindow(int argc, char **argv)
{
	int windowHeight = 800;
	int windowWidth = 800;

	//create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((GLUT_SCREEN_WIDTH / 2) - (windowWidth / 2), (GLUT_SCREEN_HEIGHT / 2) - (windowHeight / 2));
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("Some window");

	glewInit();
}

void WindowCanvas::start()
{
	// register callbacks
	glutTimerFunc(0, frameTimer, 0);
	//glutIdleFunc(idle);
	glutDisplayFunc(renderScene);
	//glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);

	glutMainLoop();
}

/*
*
*/
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

void WindowCanvas::addModel(Model &model, bool forceNewVAO)
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

		//normal
		vertexArrayData.push_back(currentVertex.xNormal);
		vertexArrayData.push_back(currentVertex.yNormal);
		vertexArrayData.push_back(currentVertex.zNormal);

		//texture uv
		vertexArrayData.push_back(currentVertex.xUV);
		vertexArrayData.push_back(currentVertex.yUV);
	}

	if (!forceNewVAO)
	{
		for (int i = 0; i < vertexArrayIDs.size(); i++)
		{
			//uses same shader, include in existing vao
			if (model.shader == vertexArrayIDs[i].shaderID)
			{
				VAOInfo *currentVAOInfo = &vertexArrayIDs[i];
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
				model.vertexDataOffset = currentVAOInfo->vertexDataByteSize;
				model.indexDataOffset = currentVAOInfo->indexDataByteSize;

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

	VAOInfo info = VAOInfo(VAO, VBO, EBO,								//id's
							model.shader,								//shader program
							model.indexData.size() * sizeof(int),		//index size
							vertexArrayData.size() * sizeof(float),		//vertex size
							model.vertexData.size());					//number of vertices

	glBindVertexArray(VAO);

	// > 1 when default buffer size isn't large enough
	int bufferSizeMultiplier = ceil((vertexArrayData.size() * sizeof(float)) / (float) DEFAULT_BUFFER_SIZE);

	std::cout << bufferSizeMultiplier << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, DEFAULT_BUFFER_SIZE * bufferSizeMultiplier, &vertexArrayData[0], GL_STATIC_DRAW);

	//interpretation of data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
	glDisableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	bufferSizeMultiplier = ceil((model.indexData.size() * sizeof(int)) / (float) DEFAULT_BUFFER_SIZE);

	//element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DEFAULT_BUFFER_SIZE * bufferSizeMultiplier, &model.indexData[0], GL_STATIC_DRAW);

	vertexArrayIDs.push_back(info);

	//this model uses the vao that was pushed into the vao list right above
	model.vaoIndex = vertexArrayIDs.size() - 1;
	//only object in buffer, so no offset
	model.setVertexBufferAndArrayData(0, 0);

	glBindVertexArray(0);
	std::cout << "model created" << std::endl;
}

void WindowCanvas::setDefaultShader(GLuint shader)
{
	defaultShader = shader;
}

WindowCanvas::~WindowCanvas()
{

}

