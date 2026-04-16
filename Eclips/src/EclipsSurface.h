#ifndef ECLIPS_SURFACE_H
#define ECLIPS_SURFACE_H

#include <GLFW/glfw3.h>
#include "EclipsInstance.h"
#include "EclipsWindow.h"
#include <memory>

class EclipsSurface {
public:
	EclipsSurface(EclipsWindow& eclipsWindow);
	void createSurface(VkInstance instance);
	void destroySurface(VkInstance instance);
	VkSurfaceKHR getSurface() const { return surface; };
private:
	EclipsWindow* eclipsWindow;
	VkSurfaceKHR surface;
};


#endif // ECLIPS_SURFACE_H