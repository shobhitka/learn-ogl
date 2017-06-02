/************************************************************

   * File Name : shader.h
   * Purpose :
   * Creation Date : 28-04-2017
   * Last Modified : Friday 02 June 2017 03:03:03 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class shader {
	private:
		GLuint id;
		GLuint type;

	public:
		shader(const char *file, GLenum type);
		~shader();
		GLuint get_id();
		GLenum get_type();
};

class program {
	private:
		GLuint id;

	public:
		program(GLuint vertex, GLuint fragment);
		~program();
		GLuint get_id();
		void use();
		void set_mat4(const char *name, glm::mat4 value);
};

class texture {
	private:
		GLuint id;

	public:
		texture(const char *image_file, GLenum type, int load_type);
		~texture();
		GLuint get_id();
		void set_tex_unit(int pid, int unit);
};

#endif // __SHADER_H__
