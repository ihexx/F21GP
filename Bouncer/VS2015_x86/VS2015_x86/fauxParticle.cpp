#include "fauxParticle.h"





const glm::vec3& particle::accelerate(const glm::vec3 & acceleration, float timeDelta)
{
	//Assumed SI units: V(m/s) A(m2/s) t(s). Scaling is responsibility of caller
	// Implement Velocity and Displacement changes
	velocity = (acceleration * timeDelta) + velocity;
	position = (velocity * timeDelta) + position;
	return position; //Const ref: read only, no copy
}

const glm::vec3 & particle::force(const glm::vec3 & push, float timeDelta)
{
	// f=ma so a=f/m
	return accelerate(push/mass, timeDelta);
}

particle::particle(const glm::vec3 & pos_init,const glm::vec3 & vel_init): position(pos_init),velocity(vel_init)
{
}


particle::~particle()
{
}


emmitter::emmitter(int numParticles):numParticles(numParticles)
{
	particleVector.reserve(numParticles);
	explosionForce.reserve(numParticles);
}

void emmitter::init(glm::vec3 & position, int lifeSpan)
{
	bool immortal = false;
	if (lifeSpan < 0) {
		immortal = true;
		lifeSpan = 1;
	}
	for (int i = 0; i < numParticles; i++) {
		particleVector.push_back(particle(position));
		particleVector[i].immortal = immortal;
		particleVector[i].lifeSpan = lifeSpan;
	}
	primed = true;
}

void emmitter::explode_flatCircle(float force)
{
	assert(primed && !exploded);
	glm::vec4 firstRadius(1.0f*force, 1.0f*force, 0.0f,1.0f);
	const float angleStep = 2*M_PI/numParticles;
	const glm::vec3 rotationAxis(0.0f, 0.0f, 1.0f);
	
	for(int i = 0; i < numParticles; i++) {
		firstRadius = glm::rotate(angleStep, rotationAxis) * firstRadius;
		explosionForce.push_back(glm::vec3(firstRadius));
	}
	exploded = true;

}

void emmitter::update(float timeDelta)
{
	assert(primed && exploded);
	for (int i = 0; i < numParticles; i++) {
		particleVector[i].accelerate(explosionForce[i], timeDelta);
	}
}
