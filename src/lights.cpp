/************************************************************

 * File Name : lights.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Wednesday 07 June 2017 03:35:16 PM IST
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

#define VERTEX_SHADER			"shaders/vshader_color_cube.tux"
#define FRAGMENT_SHADER			"shaders/fshader_color_cube.tux"
#define LAMP_VERTEX_SHADER		"shaders/vshader_lamp.tux"
#define LAMP_FRAGMENT_SHADER	"shaders/fshader_lamp.tux"

static GLfloat vertices[] = {
	-0.5f,  0.5f, -0.5f, // 0
	 0.5f,  0.5f, -0.5f, // 1
	 0.5f,  0.5f,  0.5f, // 2
	-0.5f,  0.5f,  0.5f, // 3

	-0.5f, -0.5f, -0.5f, // 4
	 0.5f, -0.5f, -0.5f, // 5
	 0.5f, -0.5f,  0.5f, // 6
	-0.5f, -0.5f,  0.5f, // 7

	-0.5f,  0.5f,  0.5f, // 8
	 0.5f,  0.5f,  0.5f, // 9
	 0.5f, -0.5f,  0.5f, // 10
	-0.5f, -0.5f,  0.5f, // 11

	-0.5f,  0.5f, -0.5f, // 12
	 0.5f,  0.5f, -0.5f, // 13
	 0.5f, -0.5f, -0.5f, // 14
	-0.5f, -0.5f, -0.5f, // 15

	-0.5f,  0.5f, -0.5f, // 16
	-0.5f, -0.5f, -0.5f, // 17
	-0.5f, -0.5f,  0.5f, // 18
	-0.5f,  0.5f,  0.5f, // 19

	 0.5f,  0.5f, -0.5f, // 20
	 0.5f, -0.5f, -0.5f, // 21
	 0.5f, -0.5f,  0.5f, // 22
	 0.5f,  0.5f,  0.5f, // 23
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

static class camera *pcam = NULL;
static float delta = 0.0;

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

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	pcam->set_fov(yoffset);
}

int simple_light(GLFWwindow* window, int width, int height)
{
	class program *shaderProgram = NULL;
	class program *lampProgram = NULL;
	float curr_frame_time = 0.0f;
	float last_frame_time = 0.0f;

	// Initialise the last mouse position at the center of the screen
	lastX = width / 2; lastY = height / 2;

	// override generic callback routine
	glfwSetKeyCallback(window, key_callback);

	try {
		class shader *vertexshader = new shader(VERTEX_SHADER, GL_VERTEX_SHADER);
		class shader *fragmentshader = new shader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

		shaderProgram = new program(vertexshader->get_id(), fragmentshader->get_id());
		delete vertexshader;
		delete fragmentshader;

		vertexshader = new shader(LAMP_VERTEX_SHADER, GL_VERTEX_SHADER);
		fragmentshader = new shader(LAMP_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

		lampProgram = new program(vertexshader->get_id(), fragmentshader->get_id());
		delete vertexshader;
		delete fragmentshader;

		// disable cursor inside app window
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// register a mouse callback
		glfwSetCursorPosCallback(window, mouse_callback);

		// register mouse scroll back callback for zooming
		glfwSetScrollCallback(window, scroll_callback);

		pcam = new camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 0.05f, 100.0f);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// reuse the same VBO for lamp as well
	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glm::vec3 lightPos(1.2f, 1.0f, 1.5f);

	// render loop
	while(!glfwWindowShouldClose(window))
	{
		curr_frame_time = glfwGetTime();
		delta = curr_frame_time - last_frame_time;
		last_frame_time = curr_frame_time;

		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// create the model, view projection for cube object
		shaderProgram->use();

		shaderProgram->set_vec3("obj_color", glm::vec3(1.0f, 0.5f, 0.31f));
		shaderProgram->set_vec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 view = pcam->get_view();
		shaderProgram->set_mat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(pcam->get_fov()), GLfloat(width) / GLfloat(height),
												0.1f, 100.0f);
		shaderProgram->set_mat4("projection", projection);
		glm::mat4 model;
		shaderProgram->set_mat4("model", model);

		// render cube object
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// reuse view and projection for lamp cube object
		lampProgram->use();
		lampProgram->set_mat4("view", view);
		lampProgram->set_mat4("projection", projection);

		// transalet in the world view and scale
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampProgram->set_mat4("model", model);

		// draw the light source cube object
		glBindVertexArray(lampVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete shaderProgram;
	delete lampProgram;

	return 0;
}
