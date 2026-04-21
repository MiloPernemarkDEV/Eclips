#include "pch.h"
#include "Surface.h"

Surface::Surface(Platform& platform)
	: platform(&platform), surface(VK_NULL_HANDLE)
{
}

void Surface::createSurface(VkInstance instance)
{
	if (glfwCreateWindowSurface(instance, platform->getWindow(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void Surface::destroySurface(VkInstance instance) {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
