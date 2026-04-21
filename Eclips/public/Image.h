#pragma once
#include <vulkan/vulkan.h>
#include "Memory.h"

class Device;

class Image {
public:
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
		uint32_t mipLevels, Device& device);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
		VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, Device& device, Memory& Memory);
	void createImageViews();
private:
};

