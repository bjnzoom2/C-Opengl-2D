#include "gl2d.h"
#include <filesystem>

struct TiledRenderer {
	float backgroundSize = 1000;
	gl2d::Texture background;

	void render(gl2d::Renderer2D& renderer) {
		auto view = renderer.getViewRect();

		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {
				int xpos = x + int(view.x / backgroundSize);
				int ypos = y + int(view.y / backgroundSize);

				renderer.renderRectangle(glm::vec4(xpos, ypos, 1, 1) * backgroundSize, background);
			}
		}
	}
};