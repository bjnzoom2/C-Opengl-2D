#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"
#include "object.h"

struct gameData {
	const double GCONSTANT = 6.67430e-11;

	std::vector<Object> objects;
};

gl2d::Renderer2D renderer;
gameData data;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

Object obj1(glm::dvec2(100, 100), glm::vec2(80, 80), glm::dvec2(150, 0), 1e18);
Object obj2(glm::dvec2(600, 600), glm::vec2(80, 80), glm::dvec2(-150, 0), 1e18);

bool gameLogic(GLFWwindow *window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	for (int i = 0; i < data.objects.size(); i++) {
		data.objects[i].render(renderer);
		if (i % 2 == 0) {
			data.objects[i].step(deltatime, data.GCONSTANT, data.objects[i + 1]);
		}
		else {
			data.objects[i].step(deltatime, data.GCONSTANT, data.objects[i - 1]);
		}
	}

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

	data.objects.push_back(obj1);
	data.objects.push_back(obj2);

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