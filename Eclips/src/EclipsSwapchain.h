#ifndef ECLIPS_SWAPCHAIN_H
#define ECLIPS_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "EclipsWindow.h"
#include "EclipsQueue.h"
#include "EclipsImage.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class EclipsSwapchain {
public:
	void createSwapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface,
		EclipsQueue& eclipsQueue, GLFWwindow* window);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkFormat& getSwapchainImageFormat() { return swapChainImageFormat; };
	VkExtent2D& getSwapChainExtent() { return swapChainExtent; };
	VkSwapchainKHR& getSwapchain() { return swapChain; };
	std::vector<VkImageView>& getSwapchainImageViews() { return swapchainImageViews; };
	std::vector<VkImage>& getSwapchainImages() { return 	swapchainImages; };

	void createImageViews(EclipsImage& eclipsImage, EclipsDevice& eclipsDevice);

private:
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapchainImages;
	VkFormat swapChainImageFormat = {};
	VkExtent2D swapChainExtent = {};
	std::vector<VkImageView> swapchainImageViews;
};

#endif // ECLIPS_SWAPCHAIN_H