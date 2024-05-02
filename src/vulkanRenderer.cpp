#include "vulkanRenderer.h"  


VulkanRenderer::VulkanRenderer() {
    // Constructor implementation (if needed)
}

int VulkanRenderer::init(GLFWwindow* newWindow) {
    window = newWindow;

    if (validation.getValidationLayerState()) {
        validation.setupDebugMessenger(instance);
    }

    try {
        createInstance();
        if (validation.getValidationLayerState()) {
            validation.setupDebugMessenger(instance);
        }
        deviceManager.pickPhysicalDevice(instance);
        deviceManager.createLogicalDevice(); 
        queueManager.init(deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice());
    } catch(const std::runtime_error &e) {
        printf("ERROR: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return 0;
}

VulkanRenderer::~VulkanRenderer() {
    // Destructor implementation (if needed)
}

void VulkanRenderer::terminate(){
    if (deviceManager.getLogicalDevice() != VK_NULL_HANDLE) {
        vkDestroyDevice(deviceManager.getLogicalDevice(), nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
    if (validation.getValidationLayerState()) {
        validation.cleanup(instance);
    }
}

void VulkanRenderer::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Koprulu";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Validation layers
    std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    if (validation.getValidationLayerState()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        validation.populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;  // Chain debug messenger create info
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    // Extensions required by GLFW and potentially by the validation layers
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> instanceExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    // Check instance extensions support
    if (!checkInstanceExtensionSupport(&instanceExtensions)) {
        throw std::runtime_error("Vulkan Instance does not support required extensions!");
    }

    // Create the Vulkan instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}



bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions)
{
    // Need to get number of extensions to create array of correct size to hold extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // Createa a list of VkExtesionProperties using count
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    // Check if given extensions are in list of available extensions
    for (const auto &checkExtension: *checkExtensions)
    {
        bool hasExtension = false;
        for (const auto &extension: extensions)
        {
            if (strcmp(checkExtension, extension.extensionName) == 0)
            {
                hasExtension = true;
                break;
            }
            
        }

        if (!hasExtension)
        {
            return false;
        } 
    }
    return true;
    
}

void VulkanRenderer::setValidationEnabled(){
    validation.setValidationLayerState(true);
}
