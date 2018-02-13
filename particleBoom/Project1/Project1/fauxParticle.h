#pragma once

#include "G2L.h"
#include "billboardMask.h"
#include <vector>
#define M_PI 3.14159265358979323846

namespace G2L {
	class particle
	{
	public:
		glm::vec3 position;
		glm::vec3 velocity;
		unsigned int life = 10;
		bool immortal = true;

		//Appearance
		float size, angle, weight;
		float r, g, b, a;
		float cameraDistance;

		const glm::vec3& accelerate(glm::vec3& acceleration, float timeDelta);

		particle(const glm::vec3 & pos_init, const glm::vec3 & vel_init = glm::vec3(0.0, 0.0, 0.0));
		~particle();
	};

	class emmitter {
	private:
		float maxLife;
		int numParticles;
		G2L::billBoard particlesContainer;
		bool primed = false;
		bool exploded = false;
		std::vector<glm::vec3> explosionForce;
		glm::vec3 gravity;


	public:
		std::vector<particle> particleVector;
		emmitter(int numParticles = 360, const glm::vec3& grav = glm::vec3(0.0f, -0.0001f, 0.0f));
		void init(glm::vec3& position, int lifeSpan = 60); //prime the bomb
		void explode_sphere(float force = 1.0f); //explode it
		void update(float timeDelta, const glm::vec3& cameraPosition = glm::vec3(0, 0, 0));
		void draw();

		~emmitter();

	};


}