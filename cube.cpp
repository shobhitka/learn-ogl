/************************************************************

 * File Name : cube.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Friday 02 June 2017 02:43:54 PM IST
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

#define VERTEX_SHADER		"vshader_cube.tux"
#define FRAGMENT_SHADER		"fshader_cube.tux"
#define TEXTURE0_IMAGE_FILE	"textures/wall.jpg"
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

int simple_cube(GLFWwindow* window, int width, int height)
{
	class program *shaderProgram = NULL;
	class texture *ptexture0 = NULL;
	class texture *ptexture1 = NULL;

	try {
		class shader *vertexshader = new shader(VERTEX_SHADER, GL_VERTEX_SHADER);
		class shader *fragmentshader = new shader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

		shaderProgram = new program(vertexshader->get_id(), fragmentshader->get_id());
		delete vertexshader;
		delete fragmentshader;
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
		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ptexture0->set_tex_unit(shaderProgram->get_id(), 0);
		ptexture1->set_tex_unit(shaderProgram->get_id(), 1);

		glUseProgram(shaderProgram->get_id());

		glBindVertexArray(VAO);

		// Translate to world co-ordinates
		glm::mat4 model;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(40.0f), glm::vec3(1.0f, 0.5f, 0.0f));

		// View matrix
		glm::mat4 view;
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		// projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), GLfloat(width) / GLfloat(height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram->get_id(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram->get_id(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shaderProgram->get_id(), "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
