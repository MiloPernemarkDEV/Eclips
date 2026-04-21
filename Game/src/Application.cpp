#include "Application.h"

Application::Application()
	: renderer(instance, platform)
{
}

Application::~Application()
{
}

bool Application::init()
{
	if (!platform.init()) {
		return false;
	}

	if (!renderer.init()) {
		return false;
	}

	return true;
}

void Application::run()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(platform.getWindow())) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		glfwPollEvents();
		camera.update(platform.getWindow(), deltaTime);

		renderer.drawFrame(camera);
	}
}

void Application::end()
{
	renderer.destroyRenderer();
	platform.destroyWindow();
	glfwTerminate();
}
