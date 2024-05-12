#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cstdint>

#include "vulkanUtils.h"

class SwapChainManager {
public:

    SwapChainManager(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window)
    : m_device(device), m_surface(surface), m_window(window) {};
    ~SwapChainManager();

    SwapChainDetails getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    void createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window, const SwapChainDetails& details);
    void cleanupSwapChain(VkDevice device);

private:

    VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, GLFWwindow* window);

    VkPhysicalDevice m_device;
    VkSurfaceKHR m_surface;
    GLFWwindow* m_window;
    VkSwapchainKHR swapchain;

};
