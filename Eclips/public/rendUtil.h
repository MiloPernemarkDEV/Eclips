#pragma once 

#include <vector>
#include <string>

class rendUtil {
public:
	static std::vector<char> readFile(const std::string& filename);
	static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
private:
};