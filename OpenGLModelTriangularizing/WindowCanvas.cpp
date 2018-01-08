#include "WindowCanvas.h"
#include "Model.h"

GLuint WindowCanvas::defaultShader;

struct VAOInfo
{
	VAOInfo(GLuint vertexArrayID, GLuint shaderID, GLsizei indexSize) :
		vertexArrayID(vertexArrayID), shaderID(shaderID), indexSize(indexSize) {};

	GLuint vertexArrayID;
	GLuint shaderID;

	GLsizei indexSize;
};

std::vector<Model> models;
std::vector<VAOInfo> vertexArrayIDs;
ShaderLoader shaderLoader;

int frames = 0;

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
		glBindVertexArray(currentVAO.vertexArrayID);
		glDrawElements(GL_TRIANGLES, currentVAO.indexSize, GL_UNSIGNED_INT, 0);
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

void WindowCanvas::addModel(Model &model)
{
	//should keep models sorted
	models.push_back(model);

	if (model.shader == 0)
	{
		if (defaultShader == 0)
		{
			std::cout << "Default shader not set" << std::endl;
		}
		model.shader = defaultShader;
	}

	//no vao with matching shader found, create a new vao
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//array for the raw data
	std::vector<float> vertexArrayData;
	//vertex information from model
	std::vector<Vertex> vertexObjectArrayData = model.vertexData;
	for (int i = 0; i < vertexObjectArrayData.size(); i++)
	{
		Vertex currentVertex = vertexObjectArrayData[i];

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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayData.size() * sizeof(float), &vertexArrayData[0], GL_STATIC_DRAW);

	//interpretation of data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glDisableVertexAttribArray(2);

	//element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexData.size() * sizeof(int), &model.indexData[0], GL_STATIC_DRAW);
	
	std::cout << model.shader << std::endl;
	vertexArrayIDs.push_back(VAOInfo(VAO, model.shader, model.indexData.size()));
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

