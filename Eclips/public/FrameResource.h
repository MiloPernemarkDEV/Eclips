#pragma once 

#include <vulkan/vulkan.h>
#include "Swapchain.h"
#include "Image.h"
#include "Device.h"
#include "rendUtil.h"
#include "RenderPass.h"

class FrameResource {
public:
	void createResources();
	FrameResource();
	FrameResource(Device& device, Image& image, Swapchain& swapchain, RenderPass& renderPass);
	VkSampleCountFlagBits getMaxUsableSampleCount();
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, 
		VkImageTiling tiling, VkFormatFeatureFlags features);
	std::vector<VkFramebuffer>& getSwapchainFrameBuffers() { return swapchainFramebuffers; };
	void cleanupFrameResource();
	void createColorResources();
	void createDepthResources();
	void createFrameBuffers();
private:
	

	Device* device;
	Image* image;
	Swapchain* swapchain;
	RenderPass* renderPass;

	VkImageView colorImageView = VK_NULL_HANDLE;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage = VK_NULL_HANDLE;
	VkDeviceMemory colorImageMemory = VK_NULL_HANDLE;

	// Depth buffer
	VkImage depthImage = VK_NULL_HANDLE;
	VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
	VkImageView depthImageView = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> swapchainFramebuffers;
};