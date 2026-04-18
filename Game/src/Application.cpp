#include "Application.h"

Application::Application()
	: eclipsRenderer(eclipsInstance, eclipsWindow)
{
}

Application::~Application()
{
}

bool Application::init()
{
	if (!eclipsWindow.init()) {
		return false;
	}

	if (!eclipsRenderer.init()) {
		return false;
	}

	return true;
}

void Application::run()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(eclipsWindow.getWindow())) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		glfwPollEvents();
		camera.update(eclipsWindow.getWindow(), deltaTime);

		eclipsRenderer.drawFrame(camera);
	}
}

void Application::end()
{
	eclipsRenderer.destroyRenderer();
	eclipsWindow.destroyWindow();
	glfwTerminate();
}
