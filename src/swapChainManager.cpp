#include "swapChainManager.h"

SwapChainManager::~SwapChainManager(){

}

SwapChainDetails SwapChainManager::getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
    
    SwapChainDetails swapChainDetails;

    // Capability Retrieval for the given surface on the given physical device.
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainDetails.surfaceCapabilities);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to get surface capabilities.");
    }

    // Format Retrieval for the given surface.
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        swapChainDetails.formats.resize(formatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainDetails.formats.data());
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface formats.");
        }
    }

    // Presentation mode retrieval.
    uint32_t presentationCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);
    if (presentationCount != 0) {
        swapChainDetails.presentationModes.resize(presentationCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, swapChainDetails.presentationModes.data());
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to get presentation modes.");
        }
    }

    return swapChainDetails;
}