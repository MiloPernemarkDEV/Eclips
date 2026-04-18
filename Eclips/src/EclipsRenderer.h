#ifndef ECLIPS_RENDERER_H
#define ECLIPS_RENDERER_H

#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX 
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>
#include <fstream>
#include <unordered_map>

#include "EclipsInstance.h"
#include "EclipsDebug.h"
#include "EclipsSurface.h"
#include "EclipsDevice.h"
#include "EclipsQueue.h"
#include "EclipsCommandPool.h"
#include "EclipsSwapchain.h"
#include "EclipsImage.h"
#include "EclipsMemory.h"
#include "Camera.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::string MODEL_PATH_VIKING_ROOM = "assets/spacecraft.obj";
const std::string MODEL_PATH_SPACE_SHIP = "assets/spacecraft.obj";
const std::string TEXTURE_PATH = "textures/spacecraft.png";

struct UniformBufferObject
{
	alignas(16)glm::mat4 model;
	alignas(16)glm::mat4 view;
	alignas(16)glm::mat4 proj;
};
			
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;


	// Describes how vertex data is laid out in memory so that it can be provided to the vertex shader	
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription binding_description = {};
		binding_description.binding = 0;
		binding_description.stride = sizeof(Vertex);
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return binding_description;
	}


	// Tells vulkan how to read each attribute (position, color, etc.) from a vertex.
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

class EclipsRenderer {
public:
	EclipsRenderer() = default;
	EclipsRenderer(EclipsInstance& eclipsInstance, EclipsWindow& eclipsWindow);

	bool init();
	void drawFrame(const Camera& camera);
	void destroyRenderer();

private:
	EclipsInstance* eclipsInstance;
	GLFWwindow* window;
	EclipsDebug eclipsDebug;
	EclipsSurface eclipsSurface;
	EclipsQueue eclipsQueue;
	EclipsDevice eclipsDevice;
	EclipsCommandPool eclipsCommandPool;
	EclipsSwapchain eclipsSwapchain;
	EclipsImage eclipsImage;
	EclipsMemory eclipsMemory;


	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

	//  Synchronization
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;

	// Buffers and Images 
	uint32_t m_currentFrame = 0;
	VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
	VkBuffer indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
	std::vector<VkDescriptorSet> descriptorSets;

	// Textures
	uint32_t mipLevels;
	VkImage textureImage = VK_NULL_HANDLE;
	VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;

	// Depth buffer
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	bool m_framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	void initVulkan();

	// MSAA
	void createColorResources();
	VkSampleCountFlagBits getMaxUsableSampleCount();

	void generateMipmaps(VkImage image, int32_t texWidth, VkFormat imageFormat, int32_t texHeight, uint32_t mipLevels);
	void loadModel(const std::string& MODEL_PATHS);

	bool hasStencilComponent(VkFormat format);
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createDepthResources();
	void createTextureSampler();
	void createTextureImageView();
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void createTextureImage();

	void createDescriptorSets();
	void createDescriptorPool();

	void createUniformBuffers();

	void createDescriptorSetLayout();

	void createIndexBuffer();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void createVertexBuffer();
	void recreateSwapChain();

	void createSyncObjects();

	void createCommandBuffer();
	void recordCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index);

	void createFrameBuffers();

	void createRenderPass();

	void createGraphicsPipeline();

	VkShaderModule createShaderModule(const std::vector<char>& code);
	void cleanupSwapChain();

	static std::vector<char> readFile(const std::string& filename);

	void updateUniformBuffer(uint32_t current_image, const Camera& camera);
};

#endif // ECLIPS_RENDERER_H