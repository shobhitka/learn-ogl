/************************************************************

   * File Name : shader.h
   * Purpose :
   * Creation Date : 28-04-2017
   * Last Modified : Thursday 11 May 2017 03:26:34 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

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
};

class texture {
	private:
		GLuint id;

	public:
		texture(const char *image_file, GLenum type, int load_type);
		~texture();
		GLuint get_id();
};

#endif // __SHADER_H__