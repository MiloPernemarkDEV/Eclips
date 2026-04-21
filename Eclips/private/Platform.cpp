#include "pch.h"
#include "Platform.h"

bool Platform::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Eclips", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);

	return true;
}

void Platform::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Platform*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void Platform::destroyWindow()
{
	glfwDestroyWindow(window);
}
