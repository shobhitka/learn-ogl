/************************************************************

 * File Name : cube.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Wednesday 07 June 2017 02:14:06 PM IST
 * Created By : Shobhit Kumar <kumar@shobhit.info>
 *
 * Code heavily borrowed from https://learnopengl.com

 *************************************************************/
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include <utils.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VERTEX_SHADER		"shaders/vshader_cube.tux"
#define FRAGMENT_SHADER		"shaders/fshader_cube.tux"
#define TEXTURE0_IMAGE_FILE	"textures/container.jpg"
#define TEXTURE1_IMAGE_FILE	"textures/awesomeface.png"

static GLfloat vertices[] = {
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // 0
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 1
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 2
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 3

	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 5
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // 6
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 7

	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // 8
	 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 9
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // 10
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 11

	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // 12
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 13
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // 14
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 15

	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // 16
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 17
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // 18
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 19

	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, // 20
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 21
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // 22
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 23
};

static GLuint indices [] = {
	// Top Face
	0, 1, 3,
	1, 2, 3,
	// bottom Face
	4, 5, 7,
	5, 6, 7,
	// Front face
	8, 9, 11,
	9, 10, 11,
	// Back Face
	12, 13, 15,
	13, 14, 15,
	// Left face
	16, 17, 19,
	17, 18, 19,
	// Right Face
	20, 21, 23,
	21, 22, 23
};

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

class camera *pcam = NULL;
float delta = 0.0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true,
	// closing the application
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pcam->move(delta, DIR_LEFT);

	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pcam->move(delta, DIR_RIGHT);

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pcam->move(delta, DIR_UP);

	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pcam->move(delta, DIR_DOWN);
}

static bool mouse_first_enter = true;
static float lastX, lastY;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouse_first_enter) {
		lastX = xpos;
		lastY = ypos;
		mouse_first_enter = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	pcam->mouse_move(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	pcam->set_fov(yoffset);
}

int simple_cube(GLFWwindow* window, int width, int height, int cam)
{
	class program *shaderProgram = NULL;
	class texture *ptexture0 = NULL;
	class texture *ptexture1 = NULL;
	float curr_frame_time = 0.0f;
	float last_frame_time = 0.0f;

	// Initialise the last mouse position at the center of the screen
	lastX = width / 2; lastY = height / 2;

	// override generic callback routine
	if (cam)
		glfwSetKeyCallback(window, key_callback);

	try {
		class shader *vertexshader = new shader(VERTEX_SHADER, GL_VERTEX_SHADER);
		class shader *fragmentshader = new shader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

		shaderProgram = new program(vertexshader->get_id(), fragmentshader->get_id());
		delete vertexshader;
		delete fragmentshader;

		if (cam) {
			// disable cursor inside app window
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// register a mouse callback
			glfwSetCursorPosCallback(window, mouse_callback);

			// register mouse scroll back callback for zooming
			glfwSetScrollCallback(window, scroll_callback);

			pcam = new camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
					glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 0.05f, 100.0f);
		}
	} catch (int e) {
		std::cout << "Program failed with errcode: " << e << std::endl;
		return e;
	}

	GLuint EBO;
	glGenBuffers(1, &EBO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Load and create texture
	try {
		ptexture0 = new texture(TEXTURE0_IMAGE_FILE, GL_TEXTURE_2D, SOIL_LOAD_RGB);
		ptexture1 = new texture(TEXTURE1_IMAGE_FILE, GL_TEXTURE_2D, SOIL_LOAD_RGBA);
	} catch (int e) {
		std::cout << "texture creation failed with errcode: " << e << std::endl;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		delete shaderProgram;
		return e;
	}

	while(!glfwWindowShouldClose(window))
	{
		curr_frame_time = glfwGetTime();
		delta = curr_frame_time - last_frame_time;
		last_frame_time = curr_frame_time;

		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ptexture0->set_tex_unit(shaderProgram->get_id(), 0);
		ptexture1->set_tex_unit(shaderProgram->get_id(), 1);

		glUseProgram(shaderProgram->get_id());

		glBindVertexArray(VAO);

		glm::mat4 view;
		if (cam) {
			view = pcam->get_view();
		} else {
			// View matrix
			// note that we're translating the scene in the reverse direction of where we want to move
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		}
		shaderProgram->set_mat4("view", view);

		// projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(pcam->get_fov()), GLfloat(width) / GLfloat(height), 0.1f, 100.0f);
		shaderProgram->set_mat4("projection", projection);

		for (int i = 0; i < 10; i++) {
			// Translate to world co-ordinates
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f, GLfloat(i/10.0)));
			shaderProgram->set_mat4("model", model);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete shaderProgram;
	delete ptexture0;
	delete ptexture1;

	return 0;
}
