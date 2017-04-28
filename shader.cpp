/************************************************************

   * File Name : shader.cpp
   * Purpose :
   * Creation Date : 28-04-2017
   * Last Modified : Friday 28 April 2017 07:36:05 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <sstream>  
#include <fstream>
#include <stdlib.h>
#include "shader.h"

using namespace std;

shader::shader(const char *file, GLenum type)
{
	const char *sname;
	switch(type) {
		case GL_VERTEX_SHADER:
			sname = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			sname = "FRAGMENT";
			break;
		default:
			std::cout << "Unsupported shader type: " << type << std::endl;
			throw -1;
	};

	string shadersrc;
	stringstream shaderstring;
	ifstream shaderfile;
	shaderfile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderfile.open(file);

		// Read file's buffer contents into streams
		shaderstring << shaderfile.rdbuf();
		shaderfile.close();

		// Convert stream into GLchar array
		shadersrc = shaderstring.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::" << sname << "::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		throw -2;
	}

	const GLchar* shadercode = shadersrc.c_str();

	GLint success;
	GLchar infoLog[512];

	id = glCreateShader(type);
	glShaderSource(id, 1, &shadercode, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << sname << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		throw -2;
	}

	this->type = type;
}

GLuint shader::get_id()
{
	return id;
}

GLenum shader::get_type()
{
	return type;
}

shader::~shader()
{
	glDeleteShader(id);
}

program::program(GLuint vertex, GLuint fragment)
{
	GLint success;
	GLchar infoLog[512];
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		throw -3;
	}
}

program::~program()
{
}

GLuint program::get_id()
{
	return id;
}

void program::use()
{
	glUseProgram(id);
}
