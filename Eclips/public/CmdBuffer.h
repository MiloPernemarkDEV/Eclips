#pragma once

#include <vulkan/vulkan.h>
#include "FrameResource.h"
#include "RenderPass.h"
#include "Swapchain.h"

class CmdBuffer {
public:
	CmdBuffer(FrameResource& frameResource, RenderPass& renderPass, Swapchain& swapchain);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t image_index);
private:
	FrameResource* frameResource;
	RenderPass* renderPass;
	Swapchain* swapchain;
};