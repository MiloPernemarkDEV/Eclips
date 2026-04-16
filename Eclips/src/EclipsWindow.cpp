#include "pch.h"
#include "EclipsWindow.h"

bool EclipsWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Eclips", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);

	return true;
}

void EclipsWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<EclipsWindow*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void EclipsWindow::destroyWindow()
{
	glfwDestroyWindow(window);
}
