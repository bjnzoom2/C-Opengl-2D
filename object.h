#include "gl2d.h"

class Object {
public:
	Object() {};
	Object(glm::dvec2 objPosition) {
		position = objPosition;
	}
	Object(glm::dvec2 objPosition, glm::vec2 objSize) {
		position = objPosition;
		size = objSize;
	}
	Object(glm::dvec2 objPosition, glm::vec2 objSize, double objMass) {
		position = objPosition;
		size = objSize;
		mass = objMass;
	}
	Object(glm::dvec2 objPosition, glm::vec2 objSize, glm::dvec2 objVelocity, double objMass) {
		position = objPosition;
		size = objSize;
		velocity = objVelocity;
		mass = objMass;
	}

	glm::dvec2 position = { 350, 350 };
	glm::vec2 size = { 80, 80 };

	glm::dvec2 velocity = { 0, 0 };
	double mass = 1e18;
	double gravity = 0;

	void render(gl2d::Renderer2D& renderer) {
		renderer.renderRectangle({ position, size });
	}

	void getGravityForce(double GCONSTANT, Object& otherObj) {
		double distance = glm::distance(position, otherObj.position);
		gravity = GCONSTANT * (mass * otherObj.mass / std::pow(distance, 2));
	}

	void step(float deltatime, double GCONSTANT, Object& otherObj) {
		glm::dvec2 direction = glm::normalize(otherObj.position - position);;
		getGravityForce(GCONSTANT, otherObj);
		double accel = gravity / mass;

		velocity += direction * glm::dvec2(accel * deltatime);
		position += velocity * glm::dvec2(deltatime);
	}
};