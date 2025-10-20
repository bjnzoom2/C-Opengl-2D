#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"

struct gameData {
	glm::vec2 playerPosition = { 350, 350 };
	glm::vec2 playerSize = { 100, 100 };
	float playerRotation = 0.0f;
};

gl2d::Renderer2D renderer;
gameData data;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

void playerRotate(GLFWwindow* window, double xpos, double ypos) {
	glm::vec2 mousePos(xpos, ypos);
	glm::vec2 playerPos = data.playerPosition + data.playerSize / glm::vec2(2, 2);

	glm::vec2 mouseDirection = mousePos - playerPos;

	if (glm::length(mouseDirection) == 0) {
		mouseDirection = { 1, 0 };
	}
	else {
		mouseDirection = glm::normalize(mouseDirection);
	}

	data.playerRotation = atan2(mouseDirection.y, -mouseDirection.x);
}

bool gameLogic(GLFWwindow *window) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glm::dvec2 cursorPos = {};
	glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
	playerRotate(window, cursorPos.x, cursorPos.y);

	glm::vec2 move = {};

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		move.y = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		move.x = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		move.y = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		move.x = 1;
	}

	if (move.x != 0 || move.y != 0) {
		move = glm::normalize(move) / glm::vec2(2, 2);
		data.playerPosition += move;
	}

	renderer.renderRectangle({ data.playerPosition, data.playerSize }, { 1.0f, 1.0f, 1.0f, 1.0f }, {}, glm::degrees(data.playerRotation) + 90);
	renderer.flush();

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Opengl 2D", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to load window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	gl2d::init();
	renderer.create();

	while (!glfwWindowShouldClose(window)) {
		gameLogic(window);
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}