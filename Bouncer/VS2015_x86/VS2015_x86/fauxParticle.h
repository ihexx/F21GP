#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <vector>
#define M_PI 3.14159265358979323846

class particle
{
public:
	glm::vec3 position;
	glm::vec3 velocity;
	float mass = 10;

	unsigned int lifeSpan = 10;
	bool immortal = true;
	
	const glm::vec3& accelerate(const glm::vec3& acceleration,float timeDelta);
	const glm::vec3& force(const glm::vec3& push, float timeDelta);


	particle(const glm::vec3 & pos_init, const glm::vec3 & vel_init = glm::vec3(0.0, 0.0, 0.0));
	~particle();
};

class emmitter {
	/*
	creates new particles and initialises them. 
	
	*/
private:
	int numParticles;
	bool primed = false;
	bool exploded = false;
	std::vector<glm::vec3> explosionForce;
	
public:
	std::vector<particle> particleVector;
	emmitter(int numParticles = 360);
	void init(glm::vec3& position,int lifeSpan = -1);
	void explode_flatCircle(float force = 1.0f);
	void update(float timeDelta);
	~emmitter();

};

