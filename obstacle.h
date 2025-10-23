#include "gl2d.h"

struct Obstacle {
	glm::vec2 position = glm::vec2(-999, -999);

	void render(gl2d::Renderer2D &renderer) {
		renderer.renderRectangle({ position, 20, 20 }, { 1.0f, 0.0f, 0.0f, 1.0f });
	}

	void step(float deltatime) {
		position -= glm::vec2(deltatime * 700, 0);
	}
};