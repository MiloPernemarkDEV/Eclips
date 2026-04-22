#include "pch.h"
#include "Pipeline.h"
#include "rendUtil.h"
#include "Vertex.h"

Pipeline::Pipeline(Device& device)
	: device(device)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::create(const PipelineConfig& config)
{
	auto vert_shader_code = rendUtil::readFile(config.vertPath);
	auto frag_shader_code = rendUtil::readFile(config.fragPath);

	VkShaderModule vert_shader_module = rendUtil::createShaderModule(vert_shader_code, device.getLogicalDevice());
	VkShaderModule frag_shader_module = rendUtil::createShaderModule(frag_shader_code, device.getLogicalDevice());

	// Shader stages 
	VkPipelineShaderStageCreateInfo vert_stage{};
	vert_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_stage.module = vert_shader_module;
	vert_stage.pName = "main";

	VkPipelineShaderStageCreateInfo frag_stage{};
	frag_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_stage.module = frag_shader_module;
	frag_stage.pName = "main";

	VkPipelineShaderStageCreateInfo stages[] = { vert_stage, frag_stage };

	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
	dynamicState.pDynamicStates = dynamicStates.data();

	// Vertex Input 
	auto binding = Vertex::getBindingDescription();
	auto attributes = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInput{};
	vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInput.vertexBindingDescriptionCount = 1;
	vertexInput.pVertexBindingDescriptions = &binding;
	vertexInput.vertexAttributeDescriptionCount = (uint32_t)attributes.size();
	vertexInput.pVertexAttributeDescriptions = attributes.data();
	
	// Input Assembly	
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = config.topology;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport 
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// Rasterizer is static but move to config later for wireframe, double sided and so on
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	// Multisampling 
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = config.msaaSamples;

	// Depth Stencil
	VkPipelineDepthStencilStateCreateInfo depth{};
	depth.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth.depthTestEnable = config.enableDepthTest;
	depth.depthWriteEnable = config.enableDepthTest;
	depth.depthCompareOp = VK_COMPARE_OP_LESS;

	// Color blending 
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	colorBlendAttachment.blendEnable = config.enableBlending;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Pipeline Layout
	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 1;
	layoutInfo.pSetLayouts = config.descriptorSetLayout;

	if (vkCreatePipelineLayout(device.getLogicalDevice(), &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	// Pipeline Creation 
	VkGraphicsPipelineCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.stageCount = 2;
	info.pStages = stages;
	info.pVertexInputState = &vertexInput;
	info.pInputAssemblyState = &inputAssembly;
	info.pViewportState = &viewportState;
	info.pRasterizationState = &rasterizer;
	info.pMultisampleState = &multisampling;
	info.pDepthStencilState = &depth;
	info.pColorBlendState = &colorBlending;
	info.pDynamicState = &dynamicState;

	info.layout = layout;
	info.renderPass = config.renderPass;
	info.subpass = 0;

	if (vkCreateGraphicsPipelines(device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline!");
	}

	vkDestroyShaderModule(device.getLogicalDevice(), vert_shader_module, nullptr);
	vkDestroyShaderModule(device.getLogicalDevice(), frag_shader_module, nullptr);
}