#include "gl2d.h"

class Object {
public:
	Object() {}
	Object(glm::vec4 objColor) : color(objColor) {}
	Object(glm::vec4 objColor, glm::dvec2 objPosition) : color(objColor), position(objPosition) {}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize) : color(objColor), position(objPosition), size(objSize) {}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize, double objMass) : color(objColor), position(objPosition), size(objSize), mass(objMass) {if (mass == 0) mass = 1;}
	Object(glm::vec4 objColor, glm::dvec2 objPosition, float objSize, double objMass, glm::dvec2 objVelocity) : color(objColor), position(objPosition), size(objSize), mass(objMass),
	velocity(objVelocity) {if (mass == 0) mass = 1;}

	glm::dvec2 position = { 350, 350 };
	float size = 40;

	glm::dvec2 velocity = {};
	double mass = 5e17;

	glm::dvec2 accumulatedForce = {};

	glm::vec4 color = { 1, 1, 1, 1 };

	void render(gl2d::Renderer2D& renderer) {
		renderer.renderCircleOutline(position, size, color, 4, 100);
	}

	void getAccumulatedForce(double GCONSTANT, Object& otherObj) {
		glm::dvec2 direction = glm::normalize(otherObj.position - position);
		double distance = glm::distance(position, otherObj.position);
		if (distance <= 1) return;

		double Fmag = GCONSTANT * (mass * otherObj.mass / (distance * distance));
		accumulatedForce += direction * Fmag;

		glm::dvec2 normal = (position - otherObj.position) / distance;
		if (distance <= size + otherObj.size) {
			glm::dvec2 tangent(normal.y, -normal.x);

			double relativeSpeedAlongNormal = glm::dot(velocity - otherObj.velocity, normal);
			double relativeSpeedAlongTangent = glm::dot(velocity - otherObj.velocity, tangent);

			glm::dvec2 collisionForce = {};
			collisionForce += relativeSpeedAlongNormal / 2 * normal * 1.9;

			velocity -= collisionForce;
			otherObj.velocity += collisionForce;

			double penetration = (size + otherObj.size) - distance;
			position += normal * penetration;
		}
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