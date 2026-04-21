#pragma once
#include <GLFW/glfw3.h>
#include "Instance.h"
#include "Platform.h"
#include <memory>

class Surface {
public:
	Surface(Platform& platform);
	void createSurface(VkInstance instance);
	void destroySurface(VkInstance instance);
	VkSurfaceKHR getSurface() const { return surface; };
private:
	Platform* platform;
	VkSurfaceKHR surface;
};

