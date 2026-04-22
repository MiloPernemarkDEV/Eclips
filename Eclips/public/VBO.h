#pragma once

#include <vulkan/vulkan.h>
#include "Vertex.h"

class Deivce;
class Queue;
class CommandPool;

class VBO {
public:
	void create(Queue& queue, Device& device, CommandPool& commandPool, std::vector<Vertex>& vertices);
	VkBuffer& get() { vertexBuffer; };
private:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

};