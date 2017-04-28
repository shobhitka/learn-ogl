/************************************************************

 * File Name : triangle.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Friday 28 April 2017 07:44:33 PM IST
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
#include "shader.h"

GLfloat vertices[] = {
	// only unique veritices
	-0.3f, -0.3f, 0.0f,
	0.3f, -0.3f, 0.0f,
	0.0f, 0.3f, 0.0f,
	0.6f, 0.3f, 0.0f,
};

GLuint indices [] = {
	// first triangle
	0, 1, 2,
	1, 2, 3
	// second triangle
};

#define VERTEX_SHADER_SIMPLE_TRIANGLE	"vshader_simple.tux"
#define FRAGMENT_SHADER_SIMPLE_TRIANGLE	"fshader_simple.tux"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true,
	// closing the application
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void usage()
{
	std::cout << "./triangle [0|1]\n";
	std::cout << "1 => Enable wireframe\n";
	std::cout << "0 => Disable wireframe (default)\n";
}

int main(int argc, char *argv[])
{
	int wireframe = 0;
	if (argc > 2) {
		usage();
		exit(0);
	}

	if (argc == 1)
		wireframe = 0;
	else
		wireframe = atoi(argv[1]);

	std::cout << "Using WIREFRAME: " << wireframe << std::endl;

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Shobhit Learns OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	class program *shaderProgram = NULL;
	// Vertex Shader
	try {
		class shader *vertexshader = new shader(VERTEX_SHADER_SIMPLE_TRIANGLE, GL_VERTEX_SHADER);
		class shader *fragmentshader = new shader(FRAGMENT_SHADER_SIMPLE_TRIANGLE, GL_FRAGMENT_SHADER);

		shaderProgram = new program(vertexshader->get_id(), fragmentshader->get_id());
		delete vertexshader;
		delete fragmentshader;

	} catch (int e) {
		std::cout << "Program failed with errcode: " << e << std::endl;
		return 0;
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

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game Loop
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram->use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	delete shaderProgram;

	glfwTerminate();

	return 0;
}
