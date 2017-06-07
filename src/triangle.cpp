/************************************************************

 * File Name : triangle.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Friday 05 May 2017 07:56:08 PM IST
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

GLfloat vertices_new[] = {
	// only unique veritices
	-0.3f, -0.3f, 0.0f,
	-0.6f, 0.3f, 0.0f,
	0.0f, 0.3f, 0.0f,
};

static const GLchar* vertexShaderSource = "#version 420 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
static const GLchar* fragmentShaderSource = "#version 420 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";
static const GLchar* fragmentShaderSourceNew = "#version 420 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"}\n\0";

int simple_triangle(GLFWwindow* window, int wireframe)
{
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader New
	GLuint fragmentShaderNew;
	fragmentShaderNew = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderNew, 1, &fragmentShaderSourceNew, NULL);
	glCompileShader(fragmentShaderNew);
	glGetShaderiv(fragmentShaderNew, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShaderNew, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMEN_NEWT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgramNew;
	shaderProgramNew = glCreateProgram();
	glAttachShader(shaderProgramNew, vertexShader);
	glAttachShader(shaderProgramNew, fragmentShaderNew);
	glLinkProgram(shaderProgramNew);
	glGetProgramiv(shaderProgramNew, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgramNew, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM_NEW::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderNew);

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

	GLuint VAO_NEW;
	glGenVertexArrays(1, &VAO_NEW);

	GLuint VBO_NEW;
	glGenBuffers(1, &VBO_NEW);

	glBindVertexArray(VAO_NEW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_NEW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_new), vertices_new, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glUseProgram(shaderProgramNew);
		glBindVertexArray(VAO_NEW);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO_NEW);
	glDeleteBuffers(1, &VBO_NEW);

	return 0;
}
