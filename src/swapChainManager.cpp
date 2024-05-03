#include "swapChainManager.h"

SwapChainDetails SwapChainManager::getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    SwapChainDetails swapChainDetails;

    // Capability Retrieval for the given surface on the given physical device.
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainDetails.surfaceCapabilities);

    // Format Retrieval for the given surface.
    // If format returned, get list of formats.
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0){
        swapChainDetails.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainDetails.formats.data());
    } 

    // Presentation mode retrieval.
    uint32_t presentationCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);
    if (presentationCount != 0)
    {
        swapChainDetails.presentationModes.resize(presentationCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, swapChainDetails.presentationModes.data());
    }
    
    return SwapChainDetails();
}