// Based on GPTemplate by S.Padilla@hw.ac.uk
// Modified to demonstrate simple collisions


#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


#include "graphics.h"
#include "shapes.h"

#include "fauxParticle.h"
#include <time.h>
#include "glPerspective.h"

// FUNCTIONS
void render(double currentTime);
void update(double currentTime);
void startup();
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// VARIABLES
bool		running = true;

Graphics	myGraphics;		// Runing all the graphics in this object

Sphere		mySphere;
particle cubeParticle(glm::vec3(3.0, 3.0, -6.0));


float t = 0.001f;			// Global variable for animation
namespace params {
	clock_t prevTick;
}

namespace controls {
	bool enabled = true;
	bool moveUp = 0, moveUp_ = 0;
	bool moveDn = 0, moveDn_ = 0;
	bool moveL = 0, moveL_ = 0;
	bool moveR = 0, moveR_ = 0;
}

int main()
{
	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				//Close if something went wrong...

	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

												// Mixed graphics and update functions - declared in main for simplicity.
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);			// Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);					// Set Callback for keys

																			// MAIN LOOP run until the window is closed
	do {
		double currentTime = glfwGetTime();		// retrieve timelapse
		glfwPollEvents();						// poll callbacks
		update(currentTime);					// update (physics, animation, structures, etc)
		render(currentTime);					// call render function.

		glfwSwapBuffers(myGraphics.window);		// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(myGraphics.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(myGraphics.window) != GL_TRUE);
		_sleep(30);
	} while (running);

	myGraphics.endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void startup() {

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float)myGraphics.windowWidth / (float)myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	// Load Geometry
	mySphere.Load();
	mySphere.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 
	cubeParticle.velocity = glm::vec3(1, 0, 1)*0.001f;

	myGraphics.SetOptimisations();		// Cull and depth testing
	params::prevTick = clock();
}

void update(double currentTime) {

	float timeStep = float(clock() - params::prevTick);

	
	static const glm::vec3 gravity(0.0f, -0.0003f, 0.0f);
	glm::vec3 userin = 0.001f*((glm::vec3(0, 0, -1) * (1.0f*controls::moveUp)) + (glm::vec3(0, 0, 1) * (1.0f*controls::moveDn)) +
		(glm::vec3(1, 0, -1) * (1.0f*controls::moveR)) + (glm::vec3(-1, 0, 1) * (1.0f*controls::moveL)));

	glm::mat4 cubeProjMatrix = myGraphics.proj_matrix;

	glm::mat4 mv_matrix_sphere = glm::translate(cubeParticle.position) *glm::mat4(1.0f);
	const glm::vec3 threshPos(2, 10, -1);
	const glm::vec3 threshNeg(-2, -1.0f, -8);
	
	
	
	//%TODO: Use a loop. GLM actually supports it
	if ((cubeParticle.position.x >= threshNeg.x) && \
		(cubeParticle.position.z >= threshNeg.z) && \
		(cubeParticle.position.y >= threshNeg.y) && \
		(cubeParticle.position.x <= threshPos.x) && \
		(cubeParticle.position.z <= threshPos.z))
	{
		
		glm::vec3 newPos(cubeParticle.force(gravity+userin, timeStep));
		
		mv_matrix_sphere =
			glm::translate(newPos) *
			glm::mat4(1.0f);
	}
	else {
			if (cubeParticle.position.x < threshNeg.x) {
					cubeParticle.position.x = threshNeg.x;
					cubeParticle.velocity.x = -(cubeParticle.velocity.x)*0.9;
		}
			if
				(cubeParticle.position.z < threshNeg.z) {
				cubeParticle.position.z = threshNeg.z;
				cubeParticle.velocity.z = -(cubeParticle.velocity.z)*0.9;
			}
			if
				(cubeParticle.position.y < threshNeg.y) {
				cubeParticle.position.y = threshNeg.y;
				cubeParticle.velocity.y = abs(cubeParticle.velocity.y)*0.9;
			}
			if (cubeParticle.position.x > threshPos.x) {
				cubeParticle.position.x = threshPos.x;
				cubeParticle.velocity.x = -(cubeParticle.velocity.x)*0.9;

			}
			if (cubeParticle.position.z > threshPos.z) {
				cubeParticle.position.z = threshPos.z;
				cubeParticle.velocity.z = -(cubeParticle.velocity.z)*0.9;
			}
	}
	

	mySphere.mv_matrix = mv_matrix_sphere;
	mySphere.proj_matrix = myGraphics.proj_matrix;

	controls::enabled = false;
	params::prevTick = clock();
}

void render(double currentTime) {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw
	mySphere.Draw();

}

void onResizeCallback(GLFWwindow* window, int w, int h) {	// call everytime the window is resized
	myGraphics.windowWidth = w;
	myGraphics.windowHeight = h;

	myGraphics.aspect = (float)w / (float)h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	controls::enabled = true;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	switch (key) {
	case GLFW_KEY_UP:
		controls::moveUp = (action != GLFW_RELEASE);
		break;
	case GLFW_KEY_DOWN:
		controls::moveDn = (action != GLFW_RELEASE);
		break;
	case GLFW_KEY_LEFT:
		controls::moveL = (action != GLFW_RELEASE);
		break;
	case GLFW_KEY_RIGHT:
		controls::moveR = (action != GLFW_RELEASE);
		break;
	}
}