#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime); 
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position; 
	glm::vec3 front;
	glm::vec3 up; //The up relative to where we're looking
	glm::vec3 right;
	glm::vec3 worldUp; //Constantly pointing to the 'real' up
	
	GLfloat yaw; //Looking left and right 
	GLfloat pitch; //Looking up and down
	//GLfloat roll; would be for rotating around 

	GLfloat moveSpeed; //Key speed
	GLfloat turnSpeed; //Mouse speed

	void update();
};

