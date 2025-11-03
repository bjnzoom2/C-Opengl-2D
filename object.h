#include "gl2d.h"

class Object {
public:
	Object() {}
	Object(glm::vec4 objColor) {
		color = objColor;
	}
	Object(glm::vec4 objColor, glm::dvec2 objPosition) {
		color = objColor;
		position = objPosition;
	}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize) {
		color = objColor;
		position = objPosition;
		size = objSize;
	}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize, double objMass) {
		color = objColor;
		position = objPosition;
		size = objSize;
		mass = objMass;
	}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize, double objMass, glm::dvec2 objVelocity) {
		color = objColor;
		position = objPosition;
		size = objSize;
		velocity = objVelocity;
		mass = objMass;
	}

	glm::dvec2 position = { 350, 350 };
	float size = 40;

	glm::dvec2 velocity = { 0, 0 };
	double mass = 5e17;

	glm::dvec2 accumulatedForce = { 0, 0 };

	glm::vec4 color = { 1, 1, 1, 1 };

	void render(gl2d::Renderer2D& renderer) {
		renderer.renderCircleOutline(position, size, color, 4, 100);
	}

	void getAccumulatedForce(double GCONSTANT, Object& otherObj) {
		glm::dvec2 direction = glm::normalize(otherObj.position - position);
		double distance = glm::distance(position, otherObj.position);

		double Fmag = GCONSTANT * (mass * otherObj.mass / (distance * distance));
		accumulatedForce += direction * Fmag;
	}

	void step(float deltatime, bool gravity) {
		glm::dvec2 accel = {};
		if (gravity) {
			accel = accumulatedForce / mass;
		}

		velocity += accel * (double)deltatime;
		position += velocity * (double)deltatime;

		accumulatedForce = glm::dvec2(0);
	}
};