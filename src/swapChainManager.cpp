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

void SwapChainManager::createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window, const SwapChainDetails& initialDetails) {
    // Fetch the current details to ensure we're using up-to-date information.
    SwapChainDetails swapchainDetails = getSwapChainDetails(m_device, m_surface);

    // 1. Choose best surface format.
    // 2. Choose best presentation mode.
    // 3. Choose swap chain image resolution.

    // find optimal surface values for our swapchain.
    VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapchainDetails.formats);
    VkPresentModeKHR presentMode = chooseBestPresentationMode(swapchainDetails.presentationModes);
    VkExtent2D extent = chooseSwapExtent(swapchainDetails.surfaceCapabilities, window);

    // How many images are in the swap chain ? Get 1 more than the minimum to allow triple buffering.
    u_int32_t imageCount = swapchainDetails.surfaceCapabilities.minImageCount +1;

    // If image count is higher than max, clamp down to max.
    // If 0, then limitless.
    if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 && swapchainDetails.surfaceCapabilities.maxImageCount < imageCount)
    imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
    
    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
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

    // get queue family indices.
    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForSurface(physicalDevice, surface);

    // If graphics and presentation families are different, then swapchain must let images be shared between families.

    if (indices.graphicsFamily != indices.presentationFamily)
    {
        uint32_t queueFamilyIndices[] = {
            (uint32_t) indices.graphicsFamily,
            (uint32_t) indices.presentationFamily
        };

        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;                      // Image share handling
        swapChainCreateInfo.queueFamilyIndexCount = 2;                                          // Number of queues to share images between
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;                           // Array of queues to share between
    }
    else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    // if old swap chain is destroyed, this one replaces it, link old one to quickly hand over responsibilities.
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;    

    // Create the swapchain.
    VkResult result = vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
        if (result != VK_SUCCESS) {
            std::cerr << "Failed to create a swap chain! Error code: " << result << std::endl;
            switch (result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    throw std::runtime_error("Failed to create a swap chain: out of host memory.");
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    throw std::runtime_error("Failed to create a swap chain: out of device memory.");
                case VK_ERROR_INITIALIZATION_FAILED:
                    throw std::runtime_error("Failed to create a swap chain: initialization failed.");
                case VK_ERROR_DEVICE_LOST:
                    throw std::runtime_error("Failed to create a swap chain: device lost.");
                case VK_ERROR_SURFACE_LOST_KHR:
                    throw std::runtime_error("Failed to create a swap chain: surface lost.");
                case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                    throw std::runtime_error("Failed to create a swap chain: native window in use.");
                default:
                    throw std::runtime_error("Failed to create a swap chain: unknown error.");
            }
        }
                                          
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
    for(const auto &presentationMode : presentationModes)
    {
        if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentationMode;
        }
    }

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
