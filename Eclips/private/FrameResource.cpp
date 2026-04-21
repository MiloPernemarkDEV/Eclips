#include "pch.h"
#include "FrameResource.h"

void FrameResource::createResources()
{
	createColorResources();
	createDepthResources();
	createFrameBuffers();
}

FrameResource::FrameResource()
{
}

FrameResource::FrameResource(Device& device, Image& image, Swapchain& swapchain, Memory& memory, RenderPass& renderPass)
	: device(&device), image(&image), swapchain(&swapchain), memory(&memory), renderPass(&renderPass)
{
}

void FrameResource::createColorResources()
{
	VkFormat colorFormat = swapchain->getSwapchainImageFormat();

	VkExtent2D swapChainExtent = swapchain->getSwapChainExtent();
	image->createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		colorImage, colorImageMemory, *device, *memory);

	colorImageView = image->createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, *device);
}

void FrameResource::createDepthResources()
{
	VkFormat depthFormat = findDepthFormat();
	VkExtent2D swapChainExtent = swapchain->getSwapChainExtent();

	image->createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory, *device, *memory);

	depthImageView = image->createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, *device);
}

void FrameResource::createFrameBuffers() {
	std::vector<VkImageView> swapChainImageViews = swapchain->getSwapchainImageViews();
	VkExtent2D swapChainExtent = swapchain->getSwapChainExtent();
	swapchainFramebuffers.resize(swapChainImageViews.size());
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::array<VkImageView, 3> attachments = {
		colorImageView,
		depthImageView,
		swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->getRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device->getLogicalDevice(), &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

VkSampleCountFlagBits FrameResource::getMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device->getPhysicalDevice(), &physicalDeviceProperties);

	// Limits is a member struct inside the properties struct we queried that has a lot of information about the device limits
	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
		physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}

VkFormat FrameResource::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VkFormat FrameResource::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(device->getPhysicalDevice(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	// Assert
	// throw std::runtime_error("failed to find supported format!");

}

void FrameResource::cleanupFrameResource()
{
	vkDestroyImageView(device->getLogicalDevice(), colorImageView, nullptr);
	vkDestroyImage(device->getLogicalDevice(), colorImage, nullptr);
	vkFreeMemory(device->getLogicalDevice(), colorImageMemory, nullptr);
	vkDestroyImageView(device->getLogicalDevice(), depthImageView, nullptr);
	vkDestroyImage(device->getLogicalDevice(), depthImage, nullptr);
	vkFreeMemory(device->getLogicalDevice(), depthImageMemory, nullptr);

	for (auto framebuffer : swapchainFramebuffers) {
		vkDestroyFramebuffer(device->getLogicalDevice(), framebuffer, nullptr);
	}
}
