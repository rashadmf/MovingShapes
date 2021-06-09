#include "MyWindow.h"

MyWindow::MyWindow()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
}

int MyWindow::Initialise()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed.");
		glfwTerminate();
		return 1;
	}

	//Set GLFW window properties and OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Not backwards compatible for old OpenGL code
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatible for new OpenGL code

	mainWindow = glfwCreateWindow(width, height, "Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window was not created");
		glfwTerminate();
		return 1;
	}

	//Get buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set current GLEW context
	glfwMakeContextCurrent(mainWindow);

	//Handle Key and mouse inputs
	createCallBacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Disables cursor while viewing the program. Good tip to remember when making games.

	//Allow modern extension feat
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Make viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);//
}

void MyWindow::createCallBacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);

}

GLfloat MyWindow::getXChange()
{
	GLfloat amount = xChange;
	xChange = 0.0f;
	return amount;
}

GLfloat MyWindow::getYChange()
{
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

void MyWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void MyWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; //Reversed order to avoid inverted y axis 

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

MyWindow::~MyWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
