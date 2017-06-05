/************************************************************

   * File Name : shader.h
   * Purpose :
   * Creation Date : 28-04-2017
   * Last Modified : Monday 05 June 2017 03:49:57 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DIR_LEFT	1
#define DIR_RIGHT	2
#define DIR_UP		3
#define DIR_DOWN	4

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

class camera {
	private:
		glm::vec3 cam_pos;
		glm::vec3 cam_front;
		glm::vec3 cam_up;
		float pitch;
		float yaw;
		float cam_speed;
		float fov;
		float max_fov;

	public:
		camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float speed, float max_fv);
		~camera();
		void move(float delta, int direction);
		glm::mat4 get_view();
		float get_sensitivity();
		void mouse_move(float xoffset, float yoffset);
		void set_fov(float yoffset);
		float get_fov();
};

#endif // __SHADER_H__
