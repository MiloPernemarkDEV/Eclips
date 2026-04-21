#pragma once 
#include<vulkan/vulkan.h>
#include<string>

#include "Device.h"

struct PipelineConfig {
	std::string vertPath;
	std::string fragPath;

	VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	bool enableDepthTest = true;
	bool enableBlending = false;

	VkDescriptorSetLayout* descriptorSetLayout = nullptr;

	VkRenderPass renderPass = VK_NULL_HANDLE;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};

class Device;

class Pipeline {
public:
	Pipeline(Device& device);
	~Pipeline();

	void create(const PipelineConfig& config);

	VkPipeline get() const { return pipeline; }
	VkPipelineLayout getLayout() const { return layout; }

private:
	Device& device;

	VkPipeline pipeline = VK_NULL_HANDLE;
	VkPipelineLayout layout = VK_NULL_HANDLE;
};