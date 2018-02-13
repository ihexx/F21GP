#include "fauxParticle.h"
#include <algorithm>




namespace G2L {
	const glm::vec3& particle::accelerate(glm::vec3 & acceleration, float timeDelta)
	{
		//Assumed SI units: V(m/s) A(m2/s) t(s). Scaling is responsibility of caller
		// Implement Velocity and Displacement changes
		velocity = (acceleration * timeDelta) + velocity;
		position = (velocity * timeDelta) + position;
		return position; //Const ref: read only, no copy
	}

	particle::particle(const glm::vec3 & pos_init, const glm::vec3 & vel_init) : position(pos_init), velocity(vel_init)
	{
	}


	particle::~particle()
	{
	}


	emmitter::emmitter(int numParticles, const glm::vec3& grav) :numParticles(numParticles), gravity(grav) {
		particleVector.reserve(numParticles);
		explosionForce.reserve(numParticles);

		particlesContainer.colors = new GLfloat[numParticles * 4];
		particlesContainer.centers = new float[numParticles * 4];
		particlesContainer.numParticles = numParticles;


	}

	void emmitter::init(glm::vec3 & position, int lifeSpan)
	{
		maxLife = lifeSpan;
		int rando = lifeSpan;
		bool immortal = false;
		if (lifeSpan < 0) {
			immortal = true;
			lifeSpan = 10;
			maxLife = lifeSpan;
		}
		
		for (int i = 0; i < numParticles; i++) {
			particleVector.push_back(particle(position));
			particleVector[i].immortal = immortal;
			if (!immortal)  rando = 1 + rand() % lifeSpan;
			particleVector[i].life = rando ;

			// %TODO: Initialise particle appearance here
			float rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			//particleVector[i].r = 155 + (unsigned char)rando;
			particleVector[i].r = std::min(0.3f +rando ,1.0f);
			rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			particleVector[i].g = std::min(0.2f +rando,1.0f);
			rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			particleVector[i].b = std::min(rando, 1.0f);
			
			particleVector[i].a = 1.0f;
			rando = 1.0f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX*2);

			particleVector[i].size = rando;

		}
		primed = true;
	}

	void emmitter::explode_sphere(float force)
	{
		assert(primed && !exploded);
		glm::vec4 firstRadius(1.0f*force, 1.0f*force, 0.0f, 1.0f);
		const float angleStep = 2 * M_PI / numParticles;
		const glm::vec3 rotationAxis(0.0f, 0.0f, 1.0f);
		const glm::vec3 rotationAxis2(1.0f, 0.0f, 0.0f);

		for (int i = 0; i < numParticles; i++) {
			particle& p = particleVector[i];
			float rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 3); //magnitude scalar
			firstRadius = glm::rotate(angleStep*rando,rotationAxis2)*glm::rotate(angleStep*rando, rotationAxis) * firstRadius;
			rando += 0.1f;
			explosionForce.push_back(glm::vec3(firstRadius)*rando);
			p.accelerate(glm::vec3(firstRadius)*rando, 1.0f);
		}
		particlesContainer.init();
		exploded = true;

	}

	void emmitter::update(float timeDelta, const glm::vec3& cameraPosition)
	{
		assert(primed && exploded);
		for (int i = 0; i < numParticles; i++) {
			particle& p = particleVector[i];
			if ((p.life > 0) && !(p.immortal)) {
				p.life--;
				float lifeScalar = float(p.life) / maxLife;
				p.accelerate(-gravity, timeDelta);
				p.cameraDistance = glm::length(p.position - cameraPosition);

				// Fill GPU buffer
				particlesContainer.centers[(4 * i) + 0] = p.position.x;
				particlesContainer.centers[(4 * i) + 1] = p.position.y;
				particlesContainer.centers[(4 * i) + 2] = p.position.z;
				particlesContainer.centers[(4 * i) + 3] = p.size * lifeScalar;

				/*particlesContainer.centers[(4 * i) + 0] = p.r;
				particlesContainer.centers[(4 * i) + 1] = p.g;
				particlesContainer.centers[(4 * i) + 2] = p.b;
				particlesContainer.centers[(4 * i) + 3] = p.a;*/

				particlesContainer.colors[(4 * i) + 0] = p.r;
				particlesContainer.colors[(4 * i) + 1] = p.g;
				particlesContainer.colors[(4 * i) + 2] = p.b;
				particlesContainer.colors[(4 * i) + 3] = 0.1f;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameraDistance = -1.0f;
				particlesContainer.centers[(4 * i) + 2] = 10;
			}
		}
		// Uniform test
		float timeVal = glfwGetTime();
		float greenVal = (sin(timeVal) / 2.0f) + 0.5f; //range 0 to 1
		int vertexColorLocation = glGetUniformLocation(
			particlesContainer.shaderProgram,
			"timeColor");
		//assert(vertexColorLocation != -1);
		glUseProgram(particlesContainer.shaderProgram);
		glUniform4f(vertexColorLocation,
			1.0f, greenVal, 0.0f, 1.0f);
		particlesContainer.update();
	}

	void emmitter::draw()
	{
		particlesContainer.draw();
	}

	emmitter::~emmitter()
	{
		delete[] particlesContainer.centers;
		delete[] particlesContainer.colors;
	}
}