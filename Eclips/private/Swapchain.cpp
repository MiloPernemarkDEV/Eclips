#include "pch.h"
#include "Swapchain.h"

Swapchain::Swapchain(Device& device, FrameResource& frameResource, Queue& queue, Surface& surface, Image& image)
	: device(&device), frameResource(&frameResource), queue(&queue), surface(&surface), image(&image)
{
}

void Swapchain::createSwapchain(GLFWwindow* window)
{
	SwapChainSupportDetails swapchainSupport = querySwapChainSupport(surface->getSurface());

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
	VkPresentModeKHR present_mode = chooseSwapPresentMode(swapchainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities, window);

	uint32_t image_count = swapchainSupport.capabilities.minImageCount + 1;
	if (swapchainSupport.capabilities.maxImageCount > 0 && image_count > swapchainSupport.capabilities.maxImageCount) {
		image_count = swapchainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->getSurface();
	createInfo.minImageCount = image_count;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices family_indices = queue->findQueueFamilies(device->getPhysicalDevice(), surface->getSurface());
	uint32_t queueFamilyIndices[] = { family_indices.graphicsFamily.value(), family_indices.presentFamily.value() };

	if (family_indices.graphicsFamily != family_indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = present_mode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device->getLogicalDevice(), &createInfo, nullptr, &swapchainKHR) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapchainKHR, &image_count, nullptr);
	swapchainImages.resize(image_count);
	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapchainKHR, &image_count, swapchainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& mode : availablePresentModes) {
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) return mode;
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}

SwapChainSupportDetails Swapchain::querySwapChainSupport(VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->getPhysicalDevice(), surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPhysicalDevice(), surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPhysicalDevice(), surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPhysicalDevice(), surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPhysicalDevice(), surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

void Swapchain::createImageViews() {
	swapchainImageViews.resize(swapchainImages.size());

	for (uint32_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = image->createImageView(swapchainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, *device);
	}
}

void Swapchain::cleanupSwapchain()
{
	frameResource->cleanupFrameResource();

	for (auto imageView : swapchainImageViews) {
		vkDestroyImageView(device->getLogicalDevice(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(device->getLogicalDevice(), swapchainKHR, nullptr);
}

void Swapchain::recreateSwapchain(GLFWwindow* window)
{
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device->getLogicalDevice());

	cleanupSwapchain();

	createSwapchain(window);

	createImageViews();
	frameResource->createColorResources();
	frameResource->createDepthResources();
	frameResource->createFrameBuffers();

	createCommandBuffer();
}
