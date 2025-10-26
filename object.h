#include "gl2d.h"

class Object {
public:
	Object() {};
	Object(glm::dvec2 objPosition) {
		position = objPosition;
	}
	Object(glm::dvec2 objPosition, float objSize) {
		position = objPosition;
		size = objSize;
	}
	Object(glm::dvec2 objPosition, float objSize, double objMass) {
		position = objPosition;
		size = objSize;
		mass = objMass;
	}
	Object(glm::dvec2 objPosition, float objSize, double objMass, glm::dvec2 objVelocity) {
		position = objPosition;
		size = objSize;
		velocity = objVelocity;
		mass = objMass;
	}

	glm::dvec2 position = { 350, 350 };
	float size = 40;

	glm::dvec2 velocity = { 0, 0 };
	double mass = 5e17;
	double Fmag = 0;

	glm::dvec2 accumulatedForce = { 0, 0 };

	void render(gl2d::Renderer2D& renderer) {
		renderer.renderCircleOutline(position, 20, Colors_White, 4, 100);
	}

	void getAccumulatedForce(double GCONSTANT, Object& otherObj) {
		glm::dvec2 direction = glm::normalize(otherObj.position - position);
		double distance = glm::distance(position, otherObj.position);
		Fmag = GCONSTANT * (mass * otherObj.mass / (distance * distance));
		
		accumulatedForce += direction * Fmag;
	}

	void step(float deltatime) {
		glm::dvec2 accel = accumulatedForce / mass;

		velocity += accel * (double)deltatime;
		position += velocity * (double)deltatime;

		accumulatedForce = glm::dvec2(0);
	}
};