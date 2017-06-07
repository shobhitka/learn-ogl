/************************************************************

   * File Name : extern.h
   * Purpose :
   * Creation Date : 05-05-2017
   * Last Modified : Wednesday 07 June 2017 03:20:21 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#ifndef __EXTERN_H__
#define __EXTERN_H__

extern void moving_triangle(GLFWwindow* window, int wireframe);
extern int simple_triangle(GLFWwindow* window, int wireframe);
extern int colored_triangle(GLFWwindow* window, int wireframe);
extern int uniform_test(GLFWwindow* window, int wireframe);
extern int simple_texture(GLFWwindow* window);
extern int simple_cube(GLFWwindow* window, int width, int height, int camera);
extern int simple_light(GLFWwindow* window, int width, int height);

#endif
