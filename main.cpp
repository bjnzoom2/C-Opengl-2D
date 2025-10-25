 #include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"

struct gameData {
	glm::dvec2 objectPosition = { 350, 0 };
	glm::vec2 objectSize = { 80, 80 };

	glm::dvec2 velocity = { 0, 0 };
	const float GRAVITY = 9.81f;
};

gl2d::Renderer2D renderer;
gameData data;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

bool gameLogic(GLFWwindow *window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	data.velocity.y += data.GRAVITY * deltatime;
	data.objectPosition += data.velocity;

	if (data.objectPosition.y >= 720) {
		data.objectPosition.y = 720;
		data.velocity.y *= -0.6;
	}

	if (std::fabs(data.velocity.y) < 0.05f && data.objectPosition.y > 718) {
		data.velocity.y = 0.0f;
	}

	renderer.renderRectangle({ data.objectPosition, data.objectSize }, { 1.0f, 1.0f, 1.0f, 1.0f }, {}, 0);

	renderer.flush();

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

int main() {
	srand(time(NULL));

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	float deltatime = 0;
	float lastframe = 0;

	while (!glfwWindowShouldClose(window)) {
		float currentframe = glfwGetTime();
		deltatime = currentframe - lastframe;
		lastframe = currentframe;

		gameLogic(window, deltatime);
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}