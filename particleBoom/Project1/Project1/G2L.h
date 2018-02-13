#pragma once
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <assert.h>
#include <chrono>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace G2L { // Gershom's Graphics Library
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	int init();
	void processInput(GLFWwindow* window);
	extern GLFWwindow* window;
	
}