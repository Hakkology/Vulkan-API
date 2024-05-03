#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "vulkanUtils.h"

class SwapChainManager {
public:
    SwapChainManager(VkDevice device, VkSurfaceKHR surface);
    ~SwapChainManager();

    SwapChainDetails getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    void createSwapChain(VkDevice device, VkSurfaceKHR surface, const SwapChainDetails& details);
    void cleanupSwapChain(VkDevice device);  // Assuming you need to clean up resources later

private:
    void querySwapChainDetails();
};
