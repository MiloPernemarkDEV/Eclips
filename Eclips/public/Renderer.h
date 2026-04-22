#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX 
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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

#include "Instance.h"
#include "Debug.h"
#include "Surface.h"
#include "Device.h"
#include "Queue.h"
#include "CommandPool.h"
#include "Swapchain.h"
#include "Image.h"
#include "Camera.h"
#include "Platform.h"
#include "RenderPass.h"
#include "FrameResource.h"
#include "DescriptorSetLayout.h"
#include "Pipeline.h"
#include "Vertex.h"
#include "VBO.h"

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

class Renderer {
public:
	Renderer() = default;
	Renderer(Instance& eclipsInstance, Platform& platform);

	bool init();
	void drawFrame(const Camera& camera);
	void destroyRenderer();

private:
	Instance* instance;
	GLFWwindow* window;
	Debug debug;
	Surface surface;
	Queue queue;
	Device device;
	CommandPool commandPool;
	Swapchain swapchain;
	Image image;
	RenderPass renderPass;
	FrameResource frameResource;
	Pipeline pipeline;
	DescriptorSetLayout descriptorSetLayout;
	VBO vertexBuffer;

	//  Synchronization
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;

	// Buffers and Images 
	uint32_t m_currentFrame = 0;
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
		
	// Move to a mesh struct 
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	bool m_framebufferResized = false;

	void initVulkan();

	// MSAA
	void generateMipmaps(VkImage image, int32_t texWidth, VkFormat imageFormat, int32_t texHeight, uint32_t mipLevels);

	void loadModel(const std::string& MODEL_PATHS);

	bool hasStencilComponent(VkFormat format);
	void createTextureSampler();
	void createTextureImageView();
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void createTextureImage();

	void createDescriptorSets();
	void createDescriptorPool();

	void createUniformBuffers();
	void createIndexBuffer();

	void createSyncObjects();
	void createCommandBuffer();
	void updateUniformBuffer(uint32_t current_image, const Camera& camera);
};
