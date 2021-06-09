#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MyWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;

MyWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

//Program will draw our movement as fast as processors allow, deltaTime is needed to normalize our speed
GLfloat deltaTime = 0.0f; 
GLfloat lastTime = 0.0f;

//Vertex shader
static const char* vShader = "Shaders/shaders.vertex";

//Fragment shader
static const char* fShader = "Shaders/shaders.fragment";

void CreateObjs()
{
	//indexing the vertices
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders() 
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = MyWindow(800, 600);
	mainWindow.Initialise();

	CreateObjs();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Rotation and scaling variables 
	float rotater = 1.0f;
	float scaler = 1.0f;
	bool growing = true;
	float minScaler = 1.0f;
	float maxScaler = 3.5f;
	float translater = 1.0f;
	float maxTranslater = 3.5f;
	bool goingRight = true;

	//Loop until window closes
	while (!mainWindow.getShouldClose())
	{
		//Calculate how fast loop is executing
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Get and handle user inputs
		glfwPollEvents();

		//Check for mouse and keys
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange()); //Always pointing down issue

		//Growing animation
		if (growing)
		{
			scaler += 0.005f;
			if (scaler > maxScaler)
			{
				growing = false;
			}
		}
		else
		{
			scaler -= 0.005f;
			if (scaler < minScaler)
			{
				growing = true;
			}
		}

		//Rotating animation
		rotater += 0.1f;
		if (rotater > 360.0f) { rotater = 1.0f; }

		//Object movement
		if (goingRight)
		{
			translater += 0.005f;
			if (translater > maxTranslater)
			{
				goingRight = false;
			}
			
		}
		else
		{
			translater -= 0.005f;
			if (translater < maxTranslater * -1.0f)
			{
				goingRight = true;
			}
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader(); //Tells graphics card to use this shader 
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model = glm::mat4(1.0f); //Identity matrix (left starting diagonal 1's)

		//Effects come out in reverse order 
		model = glm::translate(model, glm::vec3(0.0f + translater, 0.0f, -7.0f));
		model = glm::rotate(model, rotater * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.4f, scaler * 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f); //Reset model for next shape 

		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -7.0f));
		model = glm::rotate(model, rotater * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f * scaler, 0.4f, scaler * 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}