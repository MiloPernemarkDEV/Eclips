#pragma once

#include <vulkan/vulkan.h>
#include "Swapchain.h"
#include "FrameResource.h"

class RenderPass {
public:
	RenderPass() = default;
	void createRenderPass(Device& device, Swapchain& swapchain);
	VkRenderPass getRenderPass() const { return renderPass; };
private:
	VkRenderPass renderPass = VK_NULL_HANDLE;
	FrameResource frameResource;
};