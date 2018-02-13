#include "G2L.h"

using namespace G2L;
GLFWwindow* G2L::window;
void G2L::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int G2L::init() {
	glfwInit();
	//Window Initialisation
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	G2L::window = glfwCreateWindow(800, 600, "Lesson0", NULL, NULL);
	if (G2L::window == NULL) {
		std::cout << "Window creation failed." << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	//GLAD initialisation
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialise GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return 0;
}

void G2L::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_RELEASE) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


