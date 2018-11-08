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

//variables
GLFWwindow* window;
const int width = 1024, height = 768;
float xpos_mouse=width/2;
float ypos_mouse=height/2;

float lastX = 512, lastY = 384;
float yaw = 0;
float pitch = 0;

// init camera params

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 200.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 2.0f, 0.0f);

glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
glm::mat4 Model = glm::mat4(1.0f);


//MOUSE_CONTROLS
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.005f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	std::cout << "Mouse Position: Yaw - " << yaw << "| Pitch  - " << pitch << std::endl;
}

void kill_self(GLuint &vao, GLuint &vbo, GLuint programID)
{
	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
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

	float g_vertex_buffer_data[] = { //cube
		//front
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
	};

	// One color for each vertex. 
	GLfloat g_color_buffer_data[] = {
		0.684f,  0.872f,  0.124f,
		0.795f,  0.983f,  0.235f,
		0.806f,  0.094f,  0.346f,
		0.917f,  0.105f,  0.457f,
		0.028f,  0.216f,  0.568f,
		0.139f,  0.327f,  0.679f,
		0.240f,  0.438f,  0.780f,
		0.351f,  0.549f,  0.891f
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,   // second Triangle
		1 ,2 ,5,
		2, 6, 5,
		0, 1 ,4,
		1, 4, 5,
		3, 7, 2,
		2, 6, 7,
		0, 4, 3,
		4, 3, 7,
		4, 5, 7,
		7, 6, 5
	};

	float g_vertex_buffer_data1[] = {	//pyramid
		//front
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.0f, 0.5f, 0.0f

	};

	// One color for each vertex. 
	GLfloat g_color_buffer_data1[] = {
		0.684f,  0.872f,  0.124f,
		0.795f,  0.983f,  0.235f,
		0.806f,  0.094f,  0.346f,
		0.917f,  0.105f,  0.457f,
		0.028f,  0.216f,  0.568f
	};

	unsigned int indices1[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		0, 3, 2,   // second Triangle
		0 ,1 ,4,
		1, 2, 4,
		2, 3 ,4,
		3, 0, 4,

	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);

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
	


	//Init controls mouse

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	float sensitivity = 0.05f;

	//Use Zbuffer for correct depth perception
	glEnable(GL_DEPTH_TEST);
	
	glDepthFunc(GL_LESS); //Accept fragment if it closer to the camera than the former one
	glfwSetCursorPos(window, width/2, height/2);
	//Mouse look enabled for EX2

	int drawStage = 1; //drawing has 4 stages. 1. Nothing is being drawn, 2. Draw Square, 3. Draw triangle, 4. Draw circle

	float transform_over_time = 0;
	bool transform_positive = true;
	
	int transform_stage = 0;
	int last_transform_stage = transform_stage;
	// Continously check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		//TRANSFORMATION COMPUTE OVER TIME
		if (transform_over_time >= 2.0f) {
			transform_positive = false;
		}
		else if (transform_over_time <= -2.0f) {
			transform_positive = true;
		}

		if (transform_positive) {
			transform_over_time += 0.001f;
		}
		else {
			transform_over_time -= 0.001f;
		}

		//CONTROLS BINDING
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			kill_self(vao, vbo, programID);
			return 0;
		}
		float cam_speed = 0.5f;
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			cameraPos += cam_speed * (cameraFront);
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
		if (glfwGetKey(window, GLFW_KEY_4)) {
			drawStage = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_5)) {
			drawStage = 2;
			
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

			
			
		}
		if (glfwGetKey(window, GLFW_KEY_6)) {
			drawStage = 3;
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices, GL_DYNAMIC_DRAW);

			
			
		}
		if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT)) {
			transform_stage = 0;
		}
		else if (glfwGetKey(window, GLFW_KEY_1)) {
			transform_stage = 1;
		} 
		else if (glfwGetKey(window, GLFW_KEY_2)) {
			transform_stage = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_3)) {
			transform_stage = 3;
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		float xoffset = xpos_mouse - lastX;
		float yoffset = lastY - ypos_mouse; // reversed since y-coordinates range from bottom to top

		glm::mat4 model;

		if (transform_stage == 1) {
			model = glm::translate(model, glm::vec3(transform_over_time/2, 0.0f, 0.0f)); //move to right over time
		}
		else if (transform_stage == 2) {
			model = glm::scale(model, glm::vec3(transform_over_time/5, transform_over_time/5, transform_over_time/5));
		}
		else if (transform_stage == 3) {
			model = glm::rotate(model, transform_over_time/10, glm::vec3(0.0f, 1.0f, 0.0f)); //rotate on y over time
		}
	

		//	//increase size over time

		
		glm::mat4 view;
		glm::vec3 cameraFrontTemp = cameraFront;
		cameraFrontTemp.x += yaw/1000;
		cameraFrontTemp.y += pitch/1000;
		view = glm::lookAt(cameraPos, cameraPos + cameraFrontTemp, cameraUp);

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		
		glm::mat4 trans;
		
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), (float)width / height, 0.1f, 1000.0f); //going for that quake fov

		glm::mat4 MVP = projection * view * model;

		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));
		

		//bind VAO
		glBindVertexArray(vao);
		if (drawStage == 2) {
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		else if (drawStage == 3) {
			glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
		}
	} 
	
	

	return 0;
}


