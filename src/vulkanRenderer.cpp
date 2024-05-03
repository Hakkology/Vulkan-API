#include "vulkanRenderer.h"  


VulkanRenderer::VulkanRenderer() {
    // Constructor implementation (if needed)
}

int VulkanRenderer::init(GLFWwindow* newWindow) {
    window = newWindow;
    std::cout << "Starting initialization..." << std::endl;

    // Initialize validation layer and debug messenger first if needed
    if (validation.getValidationLayerState()) {
        std::cout << "Setting up debug messenger..." << std::endl;
        validation.setupDebugMessenger(instance);
    }

    try {
        // Step 1: Create Vulkan instance
        std::cout << "Creating Vulkan instance..." << std::endl;
        createInstance();
        if (validation.getValidationLayerState()) {
            std::cout << "Setting up debug messenger post-instance creation..." << std::endl;
            validation.setupDebugMessenger(instance);
        }

        // Step 2: Create surface
        std::cout << "Creating surface..." << std::endl;
        surfaceManager = std::make_unique<SurfaceManager>(instance, window);

        // Step 3: Pick physical device
        std::cout << "Picking physical device..." << std::endl;
        deviceManager.pickPhysicalDevice(instance, surfaceManager->getSurface());

        // Step 4: Create logical device
        std::cout << "Creating logical device..." << std::endl;
        deviceManager.createLogicalDevice(surfaceManager->getSurface()); 

        // Step 5: Initialize queue manager
        std::cout << "Initializing queue manager..." << std::endl;
        queueManager.init(deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice());

    } catch(const std::runtime_error &e) {
        std::cerr << "ERROR during initialization: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Initialization successful" << std::endl;
    return 0;
}


VulkanRenderer::~VulkanRenderer() {
    // Destructor implementation (if needed)
}

void VulkanRenderer::terminate(){
    if (deviceManager.getLogicalDevice() != VK_NULL_HANDLE) {
        vkDestroyDevice(deviceManager.getLogicalDevice(), nullptr);
        deviceManager.clearLogicalDevice(); // Ensure the handle is cleared
    }

    if (surfaceManager) {
        surfaceManager.reset();
    }

    if (validation.getValidationLayerState()) {
        validation.cleanup(instance);
    }

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE; // Ensure the handle is cleared
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

    std::cout << "Supported Extensions (" << extensionCount << "):" << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

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
