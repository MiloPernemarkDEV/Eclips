#include "pch.h"
#include "EclipsSwapchain.h"
#include "EclipsQueue.h"

void EclipsSwapchain::createSwapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface,
	EclipsQueue& eclipsQueue, GLFWwindow* window)
{
	SwapChainSupportDetails swap_chain_support = querySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);
	VkPresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.presentModes);
	VkExtent2D extent = chooseSwapExtent(swap_chain_support.capabilities, window);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
	if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices family_indices = eclipsQueue.findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { family_indices.graphicsFamily.value(), family_indices.presentFamily.value() };

	if (family_indices.graphicsFamily != family_indices.presentFamily) {
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	create_info.preTransform = swap_chain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logicalDevice, &create_info, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &image_count, nullptr);
	swapchainImages.resize(image_count);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &image_count, swapchainImages.data());

	swapChainImageFormat = surface_format.format;
	swapChainExtent = extent;
}

VkSurfaceFormatKHR EclipsSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
	for (const auto& available_format : available_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
			available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return available_format;
		}
	}
	return available_formats[0];	
}

VkPresentModeKHR EclipsSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
	for (const auto& mode : available_present_modes) {
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) return mode;
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D EclipsSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actual_extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actual_extent;
	}	
}

SwapChainSupportDetails EclipsSwapchain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);
	if (present_mode_count != 0) {
		details.presentModes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.presentModes.data());
	}

	return details;
}

void EclipsSwapchain::createImageViews(EclipsImage& eclipsImage, EclipsDevice& eclipsDevice) {
	swapchainImageViews.resize(swapchainImages.size());

	for (uint32_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = eclipsImage.createImageView(swapchainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, eclipsDevice);
	}
}