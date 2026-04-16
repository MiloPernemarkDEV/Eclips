#include "pch.h"
#include "EclipsSurface.h"

EclipsSurface::EclipsSurface(EclipsWindow& eclipsWindow)
	: eclipsWindow(&eclipsWindow), surface(VK_NULL_HANDLE)
{
}

void EclipsSurface::createSurface(VkInstance instance)
{
	if (glfwCreateWindowSurface(instance, eclipsWindow->getWindow(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}

void EclipsSurface::destroySurface(VkInstance instance) {
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
