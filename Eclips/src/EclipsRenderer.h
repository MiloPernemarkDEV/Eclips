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

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

const std::string MODEL_PATH_VIKING_ROOM = "assets/viking_room.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct UniformBufferObject
{
	alignas(16)glm::mat4 model;
	alignas(16)glm::mat4 view;
	alignas(16)glm::mat4 proj;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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
	void drawFrame();
	void destroyRenderer();

private:
	EclipsInstance* eclipsInstance;
	GLFWwindow* window;
	EclipsDebug eclipsDebug;
	EclipsSurface eclipsSurface;

	// Device & Queues 
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	// Swap Chain
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat = {};
	VkExtent2D swapChainExtent = {};
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

	//  Synchronization
	VkCommandPool commandPool = VK_NULL_HANDLE;
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

	// Multisampling 
	// To remove the saw like patterns along the edges we will use multisampling 
	// That saw like effect is called aliasing, hence the word anti-aliasing that always show up make more sense now 

	// What MSAA does is that it uses multiple sample points per pixel to determine it's final color 
	// More samples lead to better results but it is also hurtful for performance 

	// in MSAA each pixel is samples in an offscreen buffer which is then rendererd to the screen 

	// Im going to start with the maximum number of samples but we need to lower that down later 
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
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createTextureImageView();
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void createTextureImage();

	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

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


	void createCommandpool();

	void createFrameBuffers();

	void createRenderPass();

	void createGraphicsPipeline();

	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createImageViews();

	void createSwapChain();

	uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);

	void cleanupSwapChain();

	void createSurface();
	void pickPhysicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createLogicalDevice();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	static std::vector<char> readFile(const std::string& filename);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void updateUniformBuffer(uint32_t current_image);
};

#endif // ECLIPS_RENDERER_H