/************************************************************

 * File Name : triangle.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Thursday 01 June 2017 06:58:33 PM IST
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

#define VERTEX_SHADER_SIMPLE_TRIANGLE	"vshader_btext.tux"
#define FRAGMENT_SHADER_SIMPLE_TRIANGLE	"fshader_btext.tux"
#define TEXTURE0_IMAGE_FILE				"textures/wall.jpg"
#define TEXTURE1_IMAGE_FILE				"textures/awesomeface.png"

static GLfloat vertices[] = {
	// only unique veritices for square
	// Positions			// Color			// texture coordinates
	-0.3f, -0.3f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,				// left bottom
	-0.3f, 0.3f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,				// left top
	0.3f, -0.3f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,				// right bottom
	0.3f, 0.3f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f,				// right top
};

static GLuint indices [] = {
	// first triangle
	0, 1, 2,
	1, 2, 3
	// second triangle
};

int simple_texture(GLFWwindow* window)
{
	class program *shaderProgram = NULL;
	class texture *ptexture0 = NULL;
	class texture *ptexture1 = NULL;

	try {
		class shader *vertexshader = new shader(VERTEX_SHADER_SIMPLE_TRIANGLE, GL_VERTEX_SHADER);
		class shader *fragmentshader = new shader(FRAGMENT_SHADER_SIMPLE_TRIANGLE, GL_FRAGMENT_SHADER);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
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

	GLfloat alpha = 0.0;
	GLfloat step = 0.0075;
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ptexture0->set_tex_unit(shaderProgram->get_id(), 0);
		ptexture1->set_tex_unit(shaderProgram->get_id(), 1);

		glUseProgram(shaderProgram->get_id());

		GLint alphaloc = glGetUniformLocation(shaderProgram->get_id(), "alpha");

		alpha = alpha + step;
		if (alpha > 1.0)
			step = -0.0075;
		else if (alpha < 0.0)
			step = 0.0075;

		glUniform1f(alphaloc, alpha);

		glm::mat4 transform;

		// rotate around z-axis by 90 degree
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 0.1));

		// scale 50%
		transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

		GLint transloc = glGetUniformLocation(shaderProgram->get_id(), "transform");
		glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
