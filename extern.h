/************************************************************

   * File Name : extern.h
   * Purpose :
   * Creation Date : 05-05-2017
   * Last Modified : Thursday 11 May 2017 03:35:39 PM IST
   * Created By : Shobhit Kumar <kumar@shobhit.info>

*************************************************************/
#ifndef __EXTERN_H__
#define __EXTERN_H__

extern void moving_triangle(GLFWwindow* window, int wireframe);
extern int simple_triangle(GLFWwindow* window, int wireframe);
extern int colored_triangle(GLFWwindow* window, int wireframe);
extern int uniform_test(GLFWwindow* window, int wireframe);
int simple_texture(GLFWwindow* window);

#endif
