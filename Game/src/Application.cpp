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
	while (!glfwWindowShouldClose(eclipsWindow.getWindow())) {
		glfwPollEvents();
		eclipsRenderer.drawFrame();
	}
}

void Application::end()
{
	eclipsRenderer.destroyRenderer();
	eclipsWindow.destroyWindow();
	glfwTerminate();
}
