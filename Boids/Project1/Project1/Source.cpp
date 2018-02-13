#include "G2L.h"
#include "billboardMask.h"
#include "fauxParticle.h"
#include "boids.h"

#define DEBUG 0
#define FRAME_RATE 3
#define TICK_RATE 5
void update();
void draw();



namespace live {
	
	
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	unsigned int len = 3 * 6;
	//G2L::triangle triad(vertices,9);
	#if DEBUG
	G2L::billBoard tri; 
	#endif
	//G2L::emmitter bToom(360,glm::vec3(0,0.0000001f,0));
	glm::vec4 leader_position(0, 0, 0,0.1);
	G2L::billBoard leader_mask;

	G2L::boidField flock_seekers(20);
	

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
	live::leader_mask.init();
	static float colors[] = { 1.0f,0.0f,1.0f,1.0f };
	live::leader_mask.colors = colors;
	live::flock_seekers.align_ = 0.0f;
	live::flock_seekers.cohese_ = 0.01f;
	live::flock_seekers.separate_ = 0.0f;
	live::flock_seekers.flock_damping_ = 5.0f;
	live::flock_seekers.init(glm::vec3(0, 0, -0.5));
	


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
	if (deltaTime > 1.0f/TICK_RATE) {
		live::prevTick = clock();
#if !DEBUG
		
		live::leader_position = glm::vec4(std::sin(live::prevTick*0.001f), std::cos(live::prevTick*0.0003f), 0,1);
		live::leader_mask.centers = &live::leader_position[0];
		live::leader_mask.update();
		

		live::flock_seekers.update(deltaTime);
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
	if (now > (1.0f / FRAME_RATE)) {
		live::frameTick = clock();
		live::leader_mask.draw();
		live::flock_seekers.draw();
	}

#else
	live::tri.draw();
#endif
}

