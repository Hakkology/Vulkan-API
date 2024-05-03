#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "vulkanUtils.h"

class SwapChainManager {
public:
    SwapChainManager(VkPhysicalDevice device, VkSurfaceKHR surface)
    : m_device(device), m_surface(surface) {};
    ~SwapChainManager();

    SwapChainDetails getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    // will implement later:
    // void createSwapChain(VkDevice device, VkSurfaceKHR surface, const SwapChainDetails& details);
    // void cleanupSwapChain(VkDevice device);  // Assuming you need to clean up resources later

private:
    VkPhysicalDevice m_device;
    VkSurfaceKHR m_surface;
    // will implement later:
    //void querySwapChainDetails();
};
