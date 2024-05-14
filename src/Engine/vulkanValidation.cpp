#include "vulkanValidation.h"

VulkanValidation::VulkanValidation() : debugMessenger(VK_NULL_HANDLE) {
}

bool VulkanValidation::setupDebugMessenger(VkInstance instance) {
    if (!enableValidationLayers) return true;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        std::cout << "Debug messenger function pointer retrieved successfully." << std::endl;
        return func(instance, &createInfo, nullptr, &debugMessenger) == VK_SUCCESS;
    } else {
        std::cerr << "Failed to retrieve debug messenger function pointer." << std::endl;
        return false;
    }
}

void VulkanValidation::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

void VulkanValidation::cleanup(VkInstance instance) {
    if (!enableValidationLayers) return;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, nullptr);
    }
}

bool VulkanValidation::getValidationLayerState(){
    return enableValidationLayers;
}

void VulkanValidation::setValidationLayerState(bool state) {
    enableValidationLayers = state;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanValidation::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}