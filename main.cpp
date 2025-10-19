#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	unsigned int windowWidth = 800;
	unsigned int windowHeight = 800;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Opengl 2D", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to load window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, windowWidth, windowHeight);

	gl2d::init();
	gl2d::Renderer2D renderer;
	renderer.create();

	while (!glfwWindowShouldClose(window)) {
		renderer.updateWindowMetrics(windowWidth, windowHeight);
		renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });
		renderer.renderRectangle({ 350, 350, 100, 100 }, { 1.0f, 1.0f, 1.0f, 1.0f }, {}, 0);
		renderer.flush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}