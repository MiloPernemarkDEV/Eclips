#include "pch.h"
#include "Instance.h"

void Instance::createInstance(std::vector<const char*> requiredExtensions, uint32_t layerCount, const std::vector<const char*> validationLayers, bool enableVal) {
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Hello Triangle";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	const auto extensions = requiredExtensions;
	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	create_info.ppEnabledExtensionNames = extensions.data();

	if (enableVal) {
		create_info.enabledLayerCount = layerCount;
		create_info.ppEnabledLayerNames = validationLayers.data();
	}

	if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}

void Instance::destroyInstance()
{
	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
		instance = VK_NULL_HANDLE;
	}
}
