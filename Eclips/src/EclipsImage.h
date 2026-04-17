#ifndef ECLIPS_IMAGE_H
#define ECLIPS_IMAGE_H

#include <vulkan/vulkan.h>
#include "EclipsMemory.h"

class EclipsDevice;

class EclipsImage {
public:
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, 
		uint32_t mipLevels, EclipsDevice& eclipsDevice);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
		VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, EclipsDevice& eclipsDevice, EclipsMemory& eclipsMemory);
	void createImageViews();
private:
};

#endif // ECLIPS_IMAGE_H