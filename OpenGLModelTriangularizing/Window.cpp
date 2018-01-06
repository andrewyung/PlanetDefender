#include <iostream>
#include <string>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Window.h"
#include "Object.h"

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 1.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glEnd();

	glutSwapBuffers();
}

void Window::createWindow(int argc, char **argv)
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
	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << " GLEW Version is 4.5\n ";
	}
	else
	{
		std::cout << "GLEW 4.5 not supported\n ";
	}

	// register callbacks
	glutDisplayFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
}



void Window::redraw()
{

}

void Window::addShader(Object &targetObject, std::string vertexFilePath, std::string fragmentFilePath)
{
}

void Window::addDefaultShader(Object &targetObject)
{

}

