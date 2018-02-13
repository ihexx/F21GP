#include "boids.h"
#define SCREEN_BOUNDS_POS 1
namespace params {
	const float spread = 0.5;
}
namespace G2L {
	
	void boid::flock_steer(std::vector<boid>& flock, float timeStep)
	{
		numNeighbours = 0;
		alignment = glm::vec3(0.0f, 0.0f, 0.0f);
		cohesion = glm::vec3(0.0f, 0.0f, 0.0f);
		separation = glm::vec3(0.0f, 0.0f, 0.0f);

		if (lockOut > 0) {
			lockOut--;
		}
		for (std::vector<boid>::iterator it =
			flock.begin();
			it != flock.end();
			it++) {
			boid & agent = *it;
			if (&agent != this) {
				auto tmp = distanceFrom(agent);
				if (tmp < neighbourgoodRadius) {
					if (tmp < boundingCircle) { collide(agent); lockOut = 10; }
					
					if (!lockOut) {
						alignment += agent.heading;
						cohesion += agent.position;
						separation += agent.position - position;
						numNeighbours++;
					}
				}
			}
		}
		if (numNeighbours != 0) {
			const float tmp = 1 / float(numNeighbours);
			alignment = ((alignment * tmp));
			cohesion = ((cohesion * tmp)-position);
			separation = -(separation * tmp);

			alignment = (
				(align_*alignment) +
				(cohese_*cohesion) +
				(separate_*separation)
			);
			float strength = glm::length(alignment) +0.0001f;
			strength = (strength > flock_damping_) ? (1/strength) : (1/(flock_damping_+0.0001f));
			
			accelerate(alignment*strength*strength/ mass, timeStep);
		}

		

	}

	float boid::distanceFrom(const boid & agent)
	{
		float tmp = std::pow(
			std::pow((agent.position.x - position.x), 2) +
			std::pow((agent.position.y - position.y), 2) +
			std::pow((agent.position.z - position.z), 2),
			0.5);

		return tmp;
	}

	void boid::move(float timeStep)
	{
		// v = (d2 - d1)/time
		// d2 = v*time + d1
		position = (heading*speed*timeStep) + position;
		auto upBound = glm::greaterThan(position,
			glm::vec3(SCREEN_BOUNDS_POS, SCREEN_BOUNDS_POS, SCREEN_BOUNDS_POS));
		auto lowBound = glm::lessThan(position,
			glm::vec3(-SCREEN_BOUNDS_POS, -SCREEN_BOUNDS_POS, -SCREEN_BOUNDS_POS));
		for (int i = 0; i < 3; i++) {
			if (upBound[i])position[i] -= 2 * SCREEN_BOUNDS_POS;
			if (lowBound[i])position[i] += 2 * SCREEN_BOUNDS_POS;
		}
	}

	void boid::accelerate(const glm::vec3 & acceleration, float timeStep)
	{
		//a = v2-v1/time
		//v2 = a*time +v1
		heading = (acceleration*timeStep) + (heading*speed);
		float newSpeed = glm::length(heading);
		speed = newSpeed > max_speed ? max_speed : newSpeed;
		heading = glm::normalize(heading);

	}

	void boid::collide(boid & victim)
	{
		//move positions back so they're only just touching
		glm::vec3 curSeparation = victim.position - position;
		glm::vec3 separation = glm::normalize(curSeparation) * boundingCircle;
		float sepStrength = 0.0001f / glm::length(curSeparation);
		position = position - ((sepStrength) * separation);

		glm::vec3 v1 = heading*speed;
		glm::vec3 v2 = (victim.heading*victim.speed);
		glm::vec3 diff = v1 - v2;
		float diff_len = glm::length(diff);
		if (diff_len < 0.00001f) {
			return;
		}
		glm::vec3 normal = diff / glm::length(diff);
		float relativeVel = glm::dot(diff, normal);

		diff = relativeVel*normal;
		v1 = v1 - diff;
		v2 = v2 + diff;
		heading = glm::normalize(v1);
		speed = glm::length(v1);
		victim.heading = glm::normalize(v2);
		victim.speed = glm::length(v2);

	}

	void boid::seek(const glm::vec3 & point, float timeStep, const float steeringGain)
	{
		//Desired_velocity = (position – target)
		//Steering = desired_velocity – velocity
		glm::vec3 desired_heading = glm::normalize(point- position);
		glm::vec3 steering = desired_heading + heading;
		//glm::vec3 steering = desired_velocity - (heading*speed);
		accelerate(steering*steeringGain/mass, timeStep);
	}

	void boid::evade(const glm::vec3 & point, float timeStep, const float steeringGain)
	{
		seek(glm::vec3(-1.0f*point), timeStep, steeringGain);
	}

	void boid::wander(const glm::vec3 & point, float timeStep, const float radius, const float steeringGain)
	{
		glm::vec3 randVec(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

		randVec = glm::normalize(randVec);
		if (glm::length(position - point) < radius) {
			

			seek(randVec*radius, timeStep,0.00001f);
		}
		else {
			seek(randVec + point, timeStep, 0.00001f);
		}
	}

	

	boid::boid() :align_(1.0f),cohese_(1.0f), separate_(0.5f),flock_damping_(0.0f),mass(20),max_speed(0.0003), neighbourgoodRadius(5.0f), boundingCircle(0.3f)
	{

	}

	boid::boid(glm::vec3 pos) :boid()
	{
		position = pos;
	}

	boid::~boid()
	{
	}

	void boidField::init(const glm::vec3 & position)
	{
		for (int i = 0; i < numBoids; i++) {
			float rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			glm::vec3 tmp = position + glm::vec3(rando, rando, rando)*static_cast <float>(params::spread);
				
			boids.push_back(boid(tmp));
			boids[i].align_ = align_;
			boids[i].cohese_ = cohese_;
			boids[i].separate_ = separate_;
			boids[i].flock_damping_ = flock_damping_;
			boids[i].speed = 0.0001f;

			colorArray[(4 * i) + 0] = std::min(0.3f + rando, 1.0f);
			rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			colorArray[(4 * i) + 1] = std::min(0.2f + rando, 1.0f);
			rando = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			colorArray[(4 * i) + 2] = std::min(rando, 1.0f);

			colorArray[(4 * i) + 3] = 1.0f;
			rando = 1.0f - static_cast <float> (rand()) / static_cast <float> (RAND_MAX * 2);
			const boid& b = boids[i];


			positionArray[(4 * i) + 3] = rando;

		}
		initialised = true;
		particlesContainer.colors = colorArray;
		particlesContainer.centers = positionArray;
		particlesContainer.numParticles = numBoids;
		particlesContainer.init();
	}

	void boidField::update(float deltaTime)
	{
		assert(initialised);
		for (int i = 0; i < numBoids; i++) {
			
			boid& b = boids[i];
			
			//p.accelerate(-gravity, timeDelta);
			glm::vec3 gravity = 0.0001f*(b.position - glm::vec3(0, 0, 0)) / (std::pow(deltaTime, 2));
			//b.accelerate(-gravity, deltaTime);
			b.flock_steer(boids,deltaTime);
			//b.evade(glm::vec3(live::leader_position), deltaTime);
			b.seek(glm::vec3(live::leader_position), deltaTime);
			//b.wander(glm::vec3(0, 0, 0), deltaTime, 0.5f);

			b.move(deltaTime);

			// Fill GPU buffer
			//Maybe try buffer overflow with: https://stackoverflow.com/questions/8443102/convert-eigen-matrix-to-c-array
			particlesContainer.centers[(4 * i) + 0] = b.position.x;
			particlesContainer.centers[(4 * i) + 1] = b.position.y;
			particlesContainer.centers[(4 * i) + 2] = b.position.z;
			//particlesContainer.centers[(4 * i) + 3] = 0.01f;

			/*particlesContainer.centers[(4 * i) + 0] = p.r;
			particlesContainer.centers[(4 * i) + 1] = p.g;
			particlesContainer.centers[(4 * i) + 2] = p.b;
			particlesContainer.centers[(4 * i) + 3] = p.a;*/
			
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

	void boidField::draw()
	{
		particlesContainer.draw();
	}

	boidField::boidField()
	{

	}

	boidField::boidField(int numBoids) :numBoids(numBoids), positionArray(new float[numBoids*4]), colorArray(new float[numBoids*4])
	{
	}

	boidField::~boidField()
	{
		delete[] positionArray;
		delete[] colorArray;

	}

}
