#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Platform.h"
#include "Queue.h"
#include "Image.h"
#include "Device.h"
#include "FrameResource.h"
#include "Image.h"


struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Swapchain {
public:
	Swapchain(Device& device, FrameResource& frameResource, Queue& queue, Surface& surface, Image& image);

	void createSwapchain(GLFWwindow* window);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

	SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface);

	VkFormat& getSwapchainImageFormat() { return swapChainImageFormat; };
	VkExtent2D& getSwapChainExtent() { return swapChainExtent; };
	VkSwapchainKHR& getSwapchain() { return swapchainKHR; };
	std::vector<VkImageView>& getSwapchainImageViews() { return swapchainImageViews; };
	std::vector<VkImage>& getSwapchainImages() { return 	swapchainImages; };

	void createImageViews();

	void cleanupSwapchain();
	void recreateSwapchain(GLFWwindow* window);

private:
	Device* device;
	FrameResource* frameResource;
	Queue* queue;
	Surface* surface;
	Image* image;

	VkSwapchainKHR swapchainKHR = VK_NULL_HANDLE;
	std::vector<VkImage> swapchainImages;
	VkFormat swapChainImageFormat = {};
	VkExtent2D swapChainExtent = {};
	std::vector<VkImageView> swapchainImageViews;
};
