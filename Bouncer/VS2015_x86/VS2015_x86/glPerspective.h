#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

class glPerspective
{
public:
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
	glPerspective();
	~glPerspective();
	
	glm::mat4 mvp(const glm::mat4& model, const glm::mat4& camView);
};

