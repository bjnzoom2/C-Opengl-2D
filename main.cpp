#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"
#include "object.h"

struct gameData {
	const double GCONSTANT = 6.67430e-11;
	float timer = 0.2;

	std::vector<Object> objects;
	glm::dvec2 cursorPos = {};
};

gl2d::Renderer2D renderer;
gl2d::Camera camera;
gameData data;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

bool gameLogic(GLFWwindow *window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });
	data.timer += deltatime;

	glm::vec2 cameraMove = {};

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraMove.y = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraMove.y = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraMove.x = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraMove.x = 1;
	}

	if (cameraMove.x != 0 || cameraMove.y != 0) {
		cameraMove = glm::normalize(cameraMove);
		camera.position += 1000 * deltatime * cameraMove;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && data.timer > 0.2) {
		glfwGetCursorPos(window, &data.cursorPos.x, &data.cursorPos.y);
		Object obj(glm::dvec2(data.cursorPos.x + camera.position.x, data.cursorPos.y + camera.position.y), 20);
		data.objects.push_back(obj);
		data.timer = 0;
	}

	for (int i = 0; i < data.objects.size(); i++) {
		data.objects[i].render(renderer);
		for (int j = 0; j < data.objects.size(); j++) {
			if (i == j) {
				continue;
			}
			data.objects[i].getAccumulatedForce(data.GCONSTANT, data.objects[j]);
		}
	}

	for (int i = 0; i < data.objects.size(); i++) {
		data.objects[i].step(deltatime);
	}

	for (int i = 0; i < data.objects.size(); i++) {
		if (data.objects[i].position.x > 20000) {
			data.objects.erase(data.objects.begin() + i);
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
	renderer.setCamera(camera);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	float deltatime = 0;
	float lastframe = 0;

	while (!glfwWindowShouldClose(window)) {
		float currentframe = glfwGetTime();
		deltatime = currentframe - lastframe;
		lastframe = currentframe;

		renderer.pushCamera(camera);
		gameLogic(window, deltatime);
		renderer.popCamera();
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}