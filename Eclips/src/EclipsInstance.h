#ifndef ECLIPS_INSTANCE_H
#define ECLIPS_INSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class EclipsInstance {
public:
	EclipsInstance() = default;
	void createInstance(std::vector<const char*> requiredExtensions, uint32_t layerCount,
		const std::vector<const char*> validationLayers, bool enableVal);
	void destroyInstance();
	VkInstance getInstance() const { return instance; };
private:
	VkInstance instance;
};

#endif // ECLIPS_INSTANCE_H