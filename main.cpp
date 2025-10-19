#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

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

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}