/*
BIG HOMEWORK EX 1

Petrache Mircea & Mosteanu Bogdan

CONTROLS:

1 - Don't draw anything
2 - Draw square at random position
3 - Draw triangle at random position
4 - Draw circle at random position

Up Arrow Key    - Move camera forward
Down Arrow Key  - Move camera backwards
Left Arrow Key  - Move camera to the left
Right Arrow Key - Move camera to the right
W Key           - Move camera up
S Key           - Move camera down

Issues:
- Circle drawing doesn't work when using the shape switching code, but works if you draw from scratch, in a new window.
Since that would be a workaround at best, we decided not to implement it, and just straight up draw what we can on the screen.
*/
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//We need that randomness
#include <ctime> 

//variables
GLFWwindow* window;
const int width = 1024, height = 768;

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides); //function for drawing circle not working atm when combined with the other 2. On empty window it works
//TODO: fix circle drawing
// init camera params

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 200.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//^ Camera vectors.

void kill_self(GLuint &vao, GLuint &vbo, GLuint programID)
{
	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

float rand_gen() { //generate num between -1 and 1
	float dec = ((float)rand() / RAND_MAX);
	if ((rand() % (10) % 2 == 0)) {
		dec = -dec;
	}
	//std::cout << "TESTRAND-" << dec << "\n";
	return dec;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	float g_vertex_buffer_data[] = {	//square
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2, 3
	};
	float g_vertex_buffer_data1[] = {	//triangle
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f
	};

	unsigned int indices1[] = {  // note that we start from 0!
		0,1,2
	};
	
	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	
	//MOUSE CONTROLS DISABLED FOR EX1

	int drawStage = 1; //drawing has 4 stages. 1. Nothing is being drawn, 2. Draw Square, 3. Draw triangle, 4. Draw circle
	float rand_x = 0;
	float rand_y = 0;
	float rand_z = 0;
	int lastStage = drawStage;

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			kill_self(vao, vbo, programID);
			return 0;
		}
		float cam_speed = 0.5f;
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			cameraPos += cam_speed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			cameraPos -= cam_speed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			cameraPos.x -= (cam_speed/100);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			cameraPos.x += (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_W)) {
			cameraPos.y +=  (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			cameraPos.y -= (cam_speed / 100);
		}
		if (glfwGetKey(window, GLFW_KEY_1)) {
			drawStage = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_2)) {
			drawStage = 2;
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_3)) {
			drawStage = 3;
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
		}
		if (glfwGetKey(window, GLFW_KEY_4)) {
			drawStage = 4;
			
			glViewport(0.0f, 0.0f, width, height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
			glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
			glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
			glOrtho(0, width, 0, height, 0, 1); // essentially set coordinate system
			glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
			glLoadIdentity(); // same as above comment
		
		}
		//^ Switch buffers
		if (glfwGetKey(window, GLFW_KEY_4) || glfwGetKey(window, GLFW_KEY_3) || glfwGetKey(window, GLFW_KEY_2)) {
			if (lastStage != drawStage) {
				rand_x = rand_gen();
				rand_y = rand_gen();
				rand_z = cameraPos.z - 200;
				lastStage = drawStage;
			}
			
		}
		//^ Pick random position if we change drawing mode

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glm::mat4 model;
		
		model = glm::translate(model, glm::vec3(rand_x, rand_y, rand_z));
		//^ Translate to random point

		glm::mat4 view;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 trans;
		trans = glm::rotate(trans, glm::radians((float)glfwGetTime()*100), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), (float)width / height, 0.1f, 1000.0f); //QUAKE-like FOV

		glm::mat4 MVP = projection * view * model;

		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		//bind VAO
		glBindVertexArray(vao);
		if (drawStage == 2) {
			glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, 0);
		}
		else if (drawStage == 3) {
			glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, 0);
		}
		else if (drawStage == 4) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			drawCircle(width / 2, height / 2, 0, 120, 36);
		}
		//
		//
		//
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();
	} 
	return 0;
}


void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 1;

	GLfloat doublePi = 2.0f * 3.14f;

	GLfloat circleVerticesX[37];
	GLfloat circleVerticesY[37];
	GLfloat circleVerticesZ[37];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat allCircleVertices[37 * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}