/************************************************************

 * File Name : triangle.cpp
 * Purpose :
 * Creation Date : 27-04-2017
 * Last Modified : Wednesday 07 June 2017 03:20:01 PM IST
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
#include <utils.h>
#include <extern.h>

using namespace std;

void reset_screen()
{
	cout << "\033[2J\033[1;1H";
}

#define TEST_SAMPLE_COUNT	9
void menu()
{
	cout << "Supported Samples\n";
	cout << "========================================\n";
	cout << "1: Simple Triangle/s/Rhomus\n";
	cout << "2: Learn Basic Uniform\n";
	cout << "3: Colored triangel\n";
	cout << "4: Animated triangle moving across screen\n";
	cout << "5: Basic texture example\n";
	cout << "6: Rotating 3D Cube\n";
	cout << "7: Rotating 3D Cube with camera view\n";
	cout << "8: Basic lights exmaple\n";
	cout << "9: Exit\n";
	cout << "========================================\n";
}

int get_int_input(const char *msg, int min, int max)
{
	char input[4];
	char *str;
redo:
	cout << msg;
	cin >> input;

	int in = strtol(input, &str, 10);
	if (in < min ||  in > max) {
		cout << "Please enter only betwen" << min << " - " << max << endl;
		goto redo;
	}   

	return in; 
}

void usage()
{
	std::cout << "gltest [0|1]\n";
	std::cout << "1 => Enable wireframe\n";
	std::cout << "0 => Disable wireframe (default)\n";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true,
	// closing the application
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv[])
{
	int wireframe = 0;
	if (argc > 2) {
		usage();
		exit(0);
	}

	if (argc == 1)
		wireframe = 0;
	else {
		wireframe = atoi(argv[1]);
		if (wireframe)
			wireframe = 1;
	}

	reset_screen();
	std::cout << "Using WIREFRAME: " << wireframe << std::endl;

	while(1) {
		menu();
		int input = get_int_input("Enter command :", 0, TEST_SAMPLE_COUNT);

		// Init GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* window = glfwCreateWindow(800, 600, "Shobhit Learns OpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, key_callback);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			glfwTerminate();
			return -1;
		}

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		switch(input) {
			case 1:
				simple_triangle(window, wireframe);
				break;
			case 2:
				uniform_test(window, wireframe);
				break;
			case 3:
				colored_triangle(window, wireframe);
				break;
			case 4:
				moving_triangle(window, wireframe);
				break;
			case 5:
				simple_texture(window);
				break;
			case 6:
				simple_cube(window, 800, 600, 0);
				break;
			case 7:
				simple_cube(window, 800, 600, 1);
				break;
			case 8:
				simple_light(window, 800, 600);
				break;
			case TEST_SAMPLE_COUNT:
				cout << ("Thanks, bye.\n");
				glfwTerminate();
				exit(1);
			default:
				cout << "Unsupported command\n";
		};

		glfwTerminate();
		cout << endl;
	}
		
	reset_screen();
}
