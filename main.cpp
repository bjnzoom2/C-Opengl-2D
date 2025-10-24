 #include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl2d.h"
#include "obstacle.h"

struct gameData {
	glm::vec2 playerPosition = { 100, 350 };
	glm::vec2 playerSize = { 80, 80 };
	float playerRotation = 0.0f;

	std::vector<Obstacle> obstacles = {};
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

	static float spawnTimer = 0.0f;
	glm::vec2 move = {};
	glm::vec2 playerPos = { data.playerPosition - data.playerSize / glm::vec2(2, 2) };

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		move.y = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		move.y = 1;
	}

	if (move.x != 0 || move.y != 0) {
		move = glm::normalize(move);
		move *= deltatime * 300;
		data.playerPosition += move;
	}

	spawnTimer += deltatime;
	while (spawnTimer >= 0.5f) {
		spawnTimer -= 0.5f;
		Obstacle obs;
		float y = 10.0f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (static_cast<float>(windowHeight) - 20.0f);
		obs.position = glm::vec2(800.0f, y);
		data.obstacles.push_back(obs);
	}


	for (int i = 0; i < (int)data.obstacles.size(); i++) {
		if (glm::distance(playerPos, data.obstacles[i].position) > 2000 || glm::distance(playerPos, data.obstacles[i].position - glm::vec2(10, 10)) <= 50) {
			data.obstacles.erase(data.obstacles.begin() + i);
			i--;
			continue;
		}

		if (glm::distance(playerPos, data.obstacles[i].position - glm::vec2(10, 10)) <= 30) {
			std::cout << "HIT" << '\n';
			data.obstacles.erase(data.obstacles.begin() + i);
			i--;
			continue;
		}

		data.obstacles[i].step(deltatime);
	}

	for (int i = 0; i < (int)data.obstacles.size(); i++) {
		data.obstacles[i].render(renderer);
	}

	renderer.renderRectangle({ data.playerPosition, data.playerSize }, { 1.0f, 1.0f, 1.0f, 1.0f }, {}, 0);

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