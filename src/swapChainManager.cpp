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

bool SwapChainManager::createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window, const SwapChainDetails& initialDetails) {
    // Check if physical device, logical device, and surface are valid
    if (physicalDevice == VK_NULL_HANDLE || logicalDevice == VK_NULL_HANDLE || surface == VK_NULL_HANDLE || window == nullptr) {
        std::cerr << "Invalid Vulkan objects or GLFW window!" << std::endl;
        return false;
    }
    
    // Fetch the current details to ensure we're using up-to-date information.
    std::cout << "Getting swap chain details..." << std::endl;
    SwapChainDetails swapchainDetails = getSwapChainDetails(physicalDevice, surface);

    // 1. Choose best surface format.
    // 2. Choose best presentation mode.
    // 3. Choose swap chain image resolution.

    // Output the surface capabilities for debugging
    std::cout << "Surface capabilities - minImageCount: " << swapchainDetails.surfaceCapabilities.minImageCount << ", maxImageCount: " << swapchainDetails.surfaceCapabilities.maxImageCount << std::endl;


    // Output chosen surface format and present mode for debugging.
    std::cout << "Chosen surface format: " << swapchainDetails.formats[0].format << std::endl;
    std::cout << "Chosen present mode: " << swapchainDetails.presentationModes[0] << std::endl;

    // Find optimal surface values for our swapchain.
    VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapchainDetails.formats);
    VkPresentModeKHR presentMode = chooseBestPresentationMode(swapchainDetails.presentationModes);
    VkExtent2D extent = chooseSwapExtent(swapchainDetails.surfaceCapabilities, window);

    // Log the chosen surface format and presentation mode for verification.
    std::cout << "Surface format chosen: " << surfaceFormat.format << std::endl;
    std::cout << "Presentation mode chosen: " << presentMode << std::endl;
    std::cout << "Extent width: " << extent.width << ", height: " << extent.height << std::endl;

    // Calculate the number of images in the swap chain. Aim for 1 more than the minimum to allow triple buffering.
    std::cout << "Calculating image count for swap chain..." << std::endl;
    uint32_t imageCount = swapchainDetails.surfaceCapabilities.minImageCount + 1;
    if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainDetails.surfaceCapabilities.maxImageCount) {
        imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
    } else if (swapchainDetails.surfaceCapabilities.maxImageCount == 0) {
        // If maxImageCount is 0, there's no upper limit on the number of images.
        // We aim for triple buffering if possible.
        imageCount = std::max(imageCount, 3u); // Set to 3 for triple buffering.
    }



    // Check if the swap chain is supported
    if (!isSwapChainAdequate(physicalDevice, surface)) {
        std::cerr << "Swap chain configuration is not supported by the device." << std::endl;
        return false;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;                                     // Swapchain format
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;                             // Swapchain color space
    swapChainCreateInfo.presentMode = presentMode;                                              // Swapchain presentation mode
    swapChainCreateInfo.imageExtent = extent;                                                   // Swapchain image extent
    swapChainCreateInfo.minImageCount = imageCount;                                             // Swapchain image count
    swapChainCreateInfo.imageArrayLayers = 1;                                                   // amount of layers for each image in chain
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;                       // what attachment images will be used as
    swapChainCreateInfo.preTransform = swapchainDetails.surfaceCapabilities.currentTransform;   // transform to perform on swap chain
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                     // Draw opaque, how to handle blending images with external graphics.
    swapChainCreateInfo.clipped = VK_TRUE;                                                      // Whether to clip parts of image not in view (behind the window, off screen)


    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForSurface(physicalDevice, surface);
    if (indices.graphicsFamily != indices.presentationFamily) {
        uint32_t queueFamilyIndices[] = {static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentationFamily)};
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;                      // Image share handling
        swapChainCreateInfo.queueFamilyIndexCount = 2;                                          // Number of queues to share images between
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;                           // Array of queues to share between
    } else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // Link to old swapchain if any to quickly hand over responsibilities.


    std::cout << "Validating swap chain parameters..." << std::endl;
    if (imageCount < swapchainDetails.surfaceCapabilities.minImageCount || (swapchainDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainDetails.surfaceCapabilities.maxImageCount)) {
        std::cerr << "Image count " << imageCount << " is out of the allowed range." << std::endl;
        return false;
    }

    if (!swapchainDetails.formats.size() || !swapchainDetails.presentationModes.size()) {
        std::cerr << "Required swap chain formats or presentation modes not available." << std::endl;
        return false;
    }

    // Print the calculated image count for debugging
    std::cout << "Final image count for swap chain: " << imageCount << std::endl;

    std::cout << "Creating the swap chain..." << std::endl;
    VkResult result = vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create a swap chain! Error code: " << result << std::endl;
        return false; // Return false if failed to create the swap chain.
    }

    std::cout << "Swap chain created successfully." << std::endl;
    return true; // Return true if swap chain is created successfully.
}


// Best format is subjective.
// format: VK_FORMAT_R8G8B8A8_UNORM ( VK_FORMAT_B8G8R8A8_UNORM as backup)
// colorSpace: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
// colorSpace => graphical theory
VkSurfaceFormatKHR SwapChainManager::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
{
    // If only 1 format available and is undefined, then this means ALL formats are available (no restrictions).
    if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    
    // If restricted, search for optimal format.
    for(const auto &format: formats)
    {
        if (format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    // If cant find optimal format, then just return first format.
    return formats[0];
}

VkPresentModeKHR SwapChainManager::chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes)
{
    // Look for mailbox presentation mode.
    // for(const auto &presentationMode : presentationModes)
    // {
    //     if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
    //     {
    //         return presentationMode;
    //     }
    // }

    // If can't find, assign FIFO_KHR.
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainManager::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, GLFWwindow* window)
{
    // we can simply return surfaceCapabilities.currentExtent.
    // Surface has our window property and it has a resolution.
    // what if min and max image size is maller than the window size ?

    // if current extent is at numeric limits, then extent can vary. 
    // Otherwise it is at the size of the window.
    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return surfaceCapabilities.currentExtent;
    }
    else
    {
        // If it varies, need to set manually.
        // Get Window size.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Create new extent using window size
        VkExtent2D newExtent = {};
        newExtent.width = static_cast<uint32_t>(width);
        newExtent.height = static_cast<uint32_t>(height);

        // Surface also defines max and min, so make sure within boundaries by clamping value.
        newExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));
        newExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));

        return newExtent;
    }
    
    
}

void SwapChainManager::cleanupSwapChain(VkDevice device) {
    if (swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        swapchain = VK_NULL_HANDLE;  // Reset the handle
    }
}

#include <iostream>

bool SwapChainManager::isSwapChainAdequate(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    // Check surface capabilities
    VkSurfaceCapabilitiesKHR capabilities;
    VkResult capResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
    if (capResult != VK_SUCCESS) {
        std::cerr << "Failed to get surface capabilities, error code: " << capResult << std::endl;
        return false;
    }

    // Check surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    VkResult formatResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
    if (formatResult != VK_SUCCESS || formats.empty()) {
        std::cerr << "Failed to get surface formats or no formats available, error code: " << formatResult << std::endl;
        return false;
    }

    // Check present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VkResult presentModeResult = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
    if (presentModeResult != VK_SUCCESS || presentModes.empty()) {
        std::cerr << "Failed to get presentation modes or no modes available, error code: " << presentModeResult << std::endl;
        return false;
    }

    // Log the counts for debugging
    std::cout << "Number of surface formats available: " << formatCount << std::endl;
    std::cout << "Number of presentation modes available: " << presentModeCount << std::endl;
    std::cout << "Maximum number of images in swap chain: " << capabilities.maxImageCount << std::endl;

    // Ensure that the formats and present modes are not empty and there is a sufficient image count
    bool isAdequate = !formats.empty() && !presentModes.empty() && (capabilities.maxImageCount > 0 || capabilities.maxImageCount == 0);
    if (!isAdequate) {
        std::cerr << "Swap chain is not adequate based on the available data." << std::endl;
    }

    return isAdequate;
}

