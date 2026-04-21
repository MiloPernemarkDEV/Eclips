#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

#include "Instance.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

class Debug {
public:
	Debug() = delete;
	Debug(bool enableValidationLayers);

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger(VkInstance instance);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
	void destroyDebug(VkInstance instance);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* p_call_back_data,
		void* p_user_data);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
		const VkAllocationCallbacks* p_allocator, VkDebugUtilsMessengerEXT* p_debug_messenger);

	bool getEnableValidationlayers() const { return enableValidationLayers; };
	VkDebugUtilsMessengerEXT DebugMessenger() { return debugMessenger; };
private:
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
		const VkAllocationCallbacks* pAllocator);

	bool enableValidationLayers;
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
};
