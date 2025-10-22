#pragma once
#include "gl2d.h"

struct Bullet {
	glm::vec2 position = {};
	glm::vec2 direction = {};

	void render(gl2d::Renderer2D& renderer) {
		float bulletDir = atan2(direction.y, -direction.x) - 90.0f;
		renderer.renderRectangle({ position - glm::vec2(5, 5), 10.0f, 10.0f}, Colors_Red, glm::vec2{0.0f, 0.0f}, bulletDir);
	}

	void step(float deltatime) {
		position += direction * deltatime * 1500.0f;
	}
};