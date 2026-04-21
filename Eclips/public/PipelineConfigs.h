#pragma once
#include "Pipeline.h"

namespace PipeConfigs {

	PipelineConfig ForwardOpaque(
		VkRenderPass renderPass,
		VkDescriptorSetLayout layout,
		VkSampleCountFlagBits msaa)
	{
		PipelineConfig config{};
		config.vertPath = "Shaders/vert.spv";
		config.fragPath = "Shaders/frag.spv";
		config.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		config.enableDepthTest = true;
		config.enableBlending = false;
		config.descriptorSetLayout = &layout;
		config.renderPass = renderPass;
		config.msaaSamples = msaa;

		return config;
	}
}