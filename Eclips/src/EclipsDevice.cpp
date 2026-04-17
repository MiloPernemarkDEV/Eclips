#include "pch.h"
#include "EclipsDevice.h"
#include "EclipsDebug.h"
#include "EclipsQueue.h"
#include "EclipsSwapchain.h"
#include <set>


EclipsDevice::EclipsDevice(EclipsSurface& eclipsSurface)
	:  eclipsSurface(&eclipsSurface), physicalDevice(VK_NULL_HANDLE), device(VK_NULL_HANDLE)
{
}

void EclipsDevice::pickPhysicalDevice(VkInstance instance, EclipsQueue& eclipsQueue, EclipsSwapchain& eclipsSwapchain)
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

	if (device_count == 0) throw std::runtime_error("failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device, eclipsQueue, eclipsSwapchain)) {
			physicalDevice = device;
			// msaaSamples = getMaxUsableSampleCount();
			break;
		}
	}

	// Just returning rn will handle later
	if (physicalDevice == VK_NULL_HANDLE) {
		return;
	}
}

bool EclipsDevice::isDeviceSuitable(VkPhysicalDevice device, EclipsQueue& eclipsQueue, EclipsSwapchain& eclipsSwapchain)
{
	QueueFamilyIndices indices = eclipsQueue.findQueueFamilies(device, eclipsSurface->getSurface());
	bool extensions_supported = checkDeviceExtensionSupport(device);

	bool swap_chain_adequate = false;
	if (extensions_supported) {
		SwapChainSupportDetails swap_chain_support = eclipsSwapchain.querySwapChainSupport(device, eclipsSurface->getSurface());
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensions_supported && swap_chain_adequate && supportedFeatures.samplerAnisotropy;
}

bool EclipsDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	std::set<std::string> required_extensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : available_extensions) required_extensions.erase(extension.extensionName);

	return required_extensions.empty();
}

void EclipsDevice::createLogicalDevice(EclipsDebug& eclipsDebug, VkSurfaceKHR surface, EclipsDevice& eclipsDevice, EclipsQueue& eclipsQueue)
{
	QueueFamilyIndices family_indices = eclipsQueue.findQueueFamilies(eclipsDevice.getPhysicalDevice(), surface);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { family_indices.graphicsFamily.value(), family_indices.presentFamily.value() };

	float queue_priority = 1.0f;
	for (uint32_t queue_family : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queue_create_info{};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		queueCreateInfos.push_back(queue_create_info);
	}

	VkPhysicalDeviceFeatures device_features{};
	device_features.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	create_info.pQueueCreateInfos = queueCreateInfos.data();
	create_info.pEnabledFeatures = &device_features;
	create_info.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	create_info.ppEnabledExtensionNames = deviceExtensions.data();

	if (eclipsDebug.getEnableValidationlayers()) {
		create_info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		create_info.ppEnabledLayerNames = validationLayers.data();
	}

	if (vkCreateDevice(physicalDevice, &create_info, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(device, family_indices.graphicsFamily.value(), 0, &eclipsQueue.getGraphicsQueue());
	vkGetDeviceQueue(device, family_indices.presentFamily.value(), 0, &eclipsQueue.getPresentQueue());
}