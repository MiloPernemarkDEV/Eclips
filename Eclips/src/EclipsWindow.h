#ifndef ECLIPS_WINDOW_H
#define ECLIPS_WINDOW_H

#include <GLFW/glfw3.h>

constexpr uint32_t WIDTH = 1280;
constexpr uint32_t HEIGHT = 720;

class EclipsWindow {
public:
	EclipsWindow() = default;
	bool init();
	GLFWwindow* getWindow() { return window; };
	void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void destroyWindow();
private:
	GLFWwindow* window = nullptr;
	bool framebufferResized;
	
};

#endif // ECLIPS_WINDOW_H