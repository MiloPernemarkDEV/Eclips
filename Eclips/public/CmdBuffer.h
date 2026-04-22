#pragma once

#include <vulkan/vulkan.h>
#include "FrameResource.h"
#include "RenderPass.h"
#include "Swapchain.h"

class Pipeline;
class VBO;

class CmdBuffer {
public:
	CmdBuffer(FrameResource& frameResource, RenderPass& renderPass, Swapchain& swapchain);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t image_index, Pipeline& pipeline, VBO& VertexBufferObject);
private:
	FrameResource* frameResource;
	RenderPass* renderPass;
	Swapchain* swapchain;
};