#include "G2L.h"
#include "billboardMask.h"
#include "fauxParticle.h"

#define DEBUG 0
#define FRAME_RATE 30.0f
#define TICK_RATE 30.0f
void update();
void draw();



namespace live {
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};
	float vertices2[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	unsigned int len = 3 * 6;
	//G2L::triangle triad(vertices,9);
	#if DEBUG
	G2L::billBoard tri; 
	#endif
	G2L::emmitter bToom(1000,glm::vec3(0,0.0000001f,0));
	clock_t prevTick;
	clock_t frameTick;
	
	
	
	
}
int main() {
	int status = G2L::init();
	assert(status == EXIT_SUCCESS);

#if DEBUG
	live::tri.init();
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

#else
	live::bToom.init(glm::vec3(0,0,-0.5),1000);
	live::bToom.explode_sphere(0.0001f);

#endif
	live::prevTick = clock();
	live::frameTick = clock();

	while (!glfwWindowShouldClose(G2L::window)) {
		update();
		draw();

		glfwSwapBuffers(G2L::window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void update() {
	G2L::processInput(G2L::window);

	float deltaTime = float(clock() - live::prevTick);
	if (deltaTime > 1 / TICK_RATE) {
		live::prevTick = clock();
#if !DEBUG
		live::bToom.update(deltaTime);
#else
		live::tri.update();
#endif
	}




}

void draw()
{
	float now = float(clock() - live::frameTick);

	glClear(GL_COLOR_BUFFER_BIT);
#if !DEBUG
	if (now > (1 / 60)) {
		live::frameTick = clock();

		live::bToom.draw();
	}

#else
	live::tri.draw();
#endif
}

