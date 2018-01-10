#include "WindowCanvas.h"
#include "Model.h"
#define BUFFER_OFFSET(offset) (static_cast<char*>(0) + (offset))

GLuint WindowCanvas::defaultShader;

struct VAOInfo
{
	VAOInfo(GLuint vaoID, GLuint vboID, GLuint eboID, GLuint shaderID, GLsizei indexDataSize, GLsizei vertexDataSize) :
		vertexArrayID(vaoID), vertexBufferID(vboID), indexBufferID(eboID), shaderID(shaderID), indexDataSize(indexDataSize), vertexDataSize(vertexDataSize){};

	bool instanced = false;
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint shaderID;

	//in bytes
	GLsizei vertexDataSize;
	GLsizei indexDataSize;
};

std::vector<VAOInfo> vertexArrayIDs;
ShaderLoader shaderLoader;

int frames = 0;


void printVertexBufferContent(GLuint bufferID)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const int FLOATS_TO_READ = 100;
	float *result = new float[FLOATS_TO_READ];
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
	int *result = new int[INTS_TO_READ];
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, INTS_TO_READ  * sizeof(int), result);

	for (int i = 0; i < INTS_TO_READ; i++)
	{
		printf("%i \n", (int) result[i]);
	}
}

void renderScene(void) {
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
		if (i == 0)
		{
			glm::mat4 trans;
			trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 0.2f));
			trans = glm::translate(trans, glm::vec3(0.05f, -0.05f, 0.0f) * ((float)(frames % 100)));
			trans = glm::rotate_slow(trans, glm::radians(1.0f) * frames, glm::vec3(1.0f, 0.0f, 1.0f));
			shaderLoader.setMat4x4(currentVAO.shaderID, "transform", trans);
		}
		//std::cout << currentVAO.indexDataSize / 4 <<  " : " << currentVAO.vertexDataSize / 9 / 4 << std::endl;
		glBindVertexArray(currentVAO.vertexArrayID);
		glDrawElements(GL_TRIANGLES, currentVAO.indexDataSize, GL_UNSIGNED_INT, 0);
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
				VAOInfo currentVAOInfo = vertexArrayIDs[i];
				glBindVertexArray(currentVAOInfo.vertexArrayID);

				glBindBuffer(GL_ARRAY_BUFFER, currentVAOInfo.vertexBufferID);
				glBufferSubData(GL_ARRAY_BUFFER, currentVAOInfo.vertexDataSize, vertexArrayData.size() * sizeof(float), &vertexArrayData[0]);
				//std::cout << currentVAOInfo.vertexBufferID << std::endl;

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentVAOInfo.indexBufferID);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentVAOInfo.indexDataSize, model.indexData.size() * sizeof(int), &model.indexData[0]);
				//std::cout << currentVAOInfo.indexBufferID << std::endl;

				//update where the data for model begins in the buffers
				model.vertexDataOffset = currentVAOInfo.vertexDataSize;
				model.indexDataOffset = currentVAOInfo.indexDataSize;

				//update size of buffers
				currentVAOInfo.vertexDataSize += vertexArrayData.size() * sizeof(float);
				currentVAOInfo.indexDataSize += model.indexData.size() * sizeof(int);

				//printVertexBufferContent(currentVAOInfo.vertexBufferID);
				printIndexBufferContent(currentVAOInfo.indexBufferID);

				glBindVertexArray(0);
				std::cout << "model added : vb" << currentVAOInfo.vertexDataSize << " : ib"  << currentVAOInfo.indexDataSize << std::endl;
				
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
							vertexArrayData.size() * sizeof(float));	//vertex size

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, info.vertexDataSize * 4, &vertexArrayData[0], GL_STATIC_DRAW);

	//interpretation of data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glDisableVertexAttribArray(2);

	//element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indexDataSize * 4, &model.indexData[0], GL_STATIC_DRAW);
	
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

unsigned int WindowCanvas::assignModelID()
{
	//return a free space
	return 0;
}

WindowCanvas::~WindowCanvas()
{

}

