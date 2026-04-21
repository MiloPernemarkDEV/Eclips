#pragma once

#include <GLFW/glfw3.h>

constexpr uint32_t WIDTH = 1920;
constexpr uint32_t HEIGHT = 1080;

class Platform {
public:
	Platform() = default;
	bool init();
	GLFWwindow* getWindow() { return window; };
	void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void destroyWindow();
private:
	GLFWwindow* window = nullptr;
	bool framebufferResized;

};