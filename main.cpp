#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "gl2d.h"
#include "object.h"
#include "tiledRenderer.h"

struct gameData {
	const double GCONSTANT = 6.67430e-11;
	float timer = 0.2;

	std::vector<Object> objects;
	glm::dvec2 cursorPos = {};

	double currentMass = 5e17;
	float currentColor[4] = { 1, 1, 1, 1 };
	glm::vec4 vecColor = { 1, 1, 1, 1 };

	bool gravityEnabled = true;
};

gl2d::Renderer2D renderer;
gl2d::Camera camera;
gl2d::Texture background;

gameData data;
TiledRenderer tiledRenderer;

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

bool gameLogic(GLFWwindow *window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });
	data.timer += deltatime;

	tiledRenderer.render(renderer);

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
		camera.position += 1600 * deltatime * cameraMove;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && data.timer > 0.2) {
		glfwGetCursorPos(window, &data.cursorPos.x, &data.cursorPos.y);
		Object obj(data.vecColor, {data.cursorPos.x + camera.position.x, data.cursorPos.y + camera.position.y}, 20, data.currentMass);
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
		data.objects[i].step(deltatime, data.gravityEnabled);
	}

	for (int i = 0; i < data.objects.size(); i++) {
		if (glm::distance(data.objects[i].position, (glm::dvec2)camera.position) > 100000) {
			data.objects.erase(data.objects.begin() + i);
			i--;
		}
	}

	renderer.flush();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");

	ImGui::Text("Objects count: %d", (int)data.objects.size());
	ImGui::InputDouble("Mass", &data.currentMass);
	ImGui::ColorEdit4("Object Color", data.currentColor);
	if (ImGui::Button("Clear objects")) {
		data.objects.clear();
	}
	ImGui::Checkbox("Gravity", &data.gravityEnabled);

	for (int i = 0; i < 4; i++) {
		data.vecColor[i] = data.currentColor[i];
	}

	ImGui::Text("\nObject data");
	ImGui::BeginChild("Scrolling", {}, ImGuiChildFlags_Border);
	for (int i = 0; i < data.objects.size(); i++) {
		ImGui::Text("Object %d: Mass = %e\nPosition = (%g, %g)", i + 1, data.objects[i].mass, data.objects[i].position.x, data.objects[i].position.y);
	}
	ImGui::EndChild();

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	gl2d::init();
	renderer.create();
	renderer.setCamera(camera);

	camera.zoom = 1;

	std::filesystem::path backgroundPath = R"(C:\Users\luken\Downloads\stars.jpg)";

	background.loadFromFile(backgroundPath.string().c_str(), false, true);

	tiledRenderer.background = background;

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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}