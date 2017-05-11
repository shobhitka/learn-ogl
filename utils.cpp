/************************************************************

   * File Name : shader.cpp
   * Purpose :
   * Creation Date : 28-04-2017
   * Last Modified : Thursday 11 May 2017 06:40:51 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <sstream>  
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <SOIL/SOIL.h>

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

texture::texture(const char *image_file, GLenum type, int load_type)
{
	if (!image_file) {
		std::cout << "ERROR::TEXTURE::CREATION_FAIL::INAVLID_PARAMS" << std::endl;
		throw -1;
	}

	switch(type) {
		case GL_TEXTURE_2D:
			break;
		case GL_TEXTURE_1D:
		case GL_TEXTURE_3D:
		default:
			std::cout << "ERROR::TEXTURE::CREATION_FAIL::UNSUPPORTED_TYPE" << std::endl;
			throw -2;
	}

	switch(load_type) {
		case SOIL_LOAD_RGB:
			break;
		case SOIL_LOAD_RGBA:
		default:
			std::cout << "ERROR::TEXTURE::CREATION_FAIL::UNSUPPORTED_IMAGE_FROMAT" << std::endl;
			throw -3;
	}

	int w, h;
	unsigned char *image = SOIL_load_image(image_file, &w, &h, 0, load_type);
	if (!image) {
		std::cout << "ERROR::TEXTURE::CREATION_FAIL::SOIL::LOAD_IMAGE_FAIL" << std::endl;
		throw -4;
	}

	// Create texture
	glGenTextures(1, &id);
	glBindTexture(type, id);

	// Set the texture wrapping parameters
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(type, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);
	SOIL_free_image_data(image);
	glBindTexture(type, 0);
}

texture::~texture()
{
}

GLuint texture::get_id()
{
	return id;
}

void texture::set_tex_unit(int pid, int unit)
{
	char szParam[12];
	memset(szParam, 0, 12);
	sprintf(szParam, "myTexture%d", unit);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
	glUniform1i(glGetUniformLocation(pid, szParam), unit);
}
