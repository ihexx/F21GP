#pragma once
#include <Eigen/Dense>
#include <vector>
#include "G2L.h"
#include "fauxParticle.h"
#include <math.h>

namespace G2L {

	//Autonomous particle
	class boid
	{
	private:
		const float neighbourgoodRadius;
		const float mass;
		const float boundingCircle;
		int lockOut = 0;

		//static const int blindSpot = 120; //degrees


	public:
		int numNeighbours = 0;

		//behavior coefficients
		const float max_speed;
		float speed;
		float align_, cohese_, separate_,flock_damping_;
		glm::vec3 position = glm::vec3(0,0,0);
		glm::vec3 heading = glm::vec3(1, 0, 0);
		
		
		glm::vec3 alignment;
		glm::vec3 cohesion;
		glm::vec3 separation;


		void boid::flock_steer(std::vector<boid>& flock, float timeStep);
		float distanceFrom(const boid& agent);
		void move(float timeStep);
		void accelerate(const glm::vec3& acceleration, float timeStep);
		void collide(boid& victim);
		void seek(const glm::vec3& point, float timeStep, const float steeringGain = 0.001);
		void evade(const glm::vec3& point, float timeStep, const float steeringGain = 0.001);
		void wander(const glm::vec3& point, float timeStep, const float radius = 0.1f, const float steeringGain = 0.001f);
		boid();
		boid(glm::vec3 position);
		~boid();
	};

	class boidField {
		bool initialised = false;
		billBoard particlesContainer;
	public:

		std::vector<boid> boids;
		int numBoids = 0;
		float align_, cohese_, separate_, flock_damping_;


		float * positionArray;
		float * colorArray;

		void init(const glm::vec3 & position);
		void update(float deltaTime);
		void draw();
		boidField();
		boidField(int numBoids);
		~boidField();

	};
}