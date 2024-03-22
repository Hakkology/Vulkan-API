#include "vulkanRenderer.h"  


VulkanRenderer::VulkanRenderer() : {
    // Constructor implementation (if needed)
}

int VulkanRenderer::init(GLFWwindow* newWindow) {
    window = newWindow;

    if (validation.getValidationLayerState()) {
        validation.setupDebugMessenger(instance);
    }

    try {
        createInstance();
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

void VulkanRenderer::createInstance(){
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    // Information about the application itself
    // Most data here does not affect the probram and is for developer convenience.
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Renderer";               // Custom name of the application.
    appInfo.applicationVersion = VK_MAKE_VERSION (1, 0, 0);     // CUstom version of the application.
    appInfo.pEngineName = "Koprulu";                            // Custom engine name
    appInfo.engineVersion = VK_MAKE_VERSION (1,0,0);            // Custom engine version
    appInfo.apiVersion = VK_API_VERSION_1_3;                    // the Vulkan Version

                                                                // Creation Info for a VkInstance (Vulkan Instance)
    VkInstanceCreateInfo createInfo = {};                       // We dont necessarily know what type of info is created. This enum is basically what its type is.
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  // createInfo.pNext is for additional structs and information parameters, stype is structure type.
    createInfo.pApplicationInfo = &appInfo;

    if (validation.getValidationLayerState()) {
        validation.populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;  // Link debugCreateInfo to createInfo
    }

    // Create a list to hold instance extensions
    std::vector<const char*> instanceExtensions = std::vector<const char*>();

    uint32_t glfwExtensionCount = 0;                            // glfw may require multiple extensions.
    const char** glfwExtensions;                                // Extensions passed as array of cstrings, so need pointer (the array) to pointer.
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // get glfw extensions
    
    // add glfw extensions to list of extensions
    for (size_t i = 0; i < glfwExtensionCount; i++)
    {
        instanceExtensions.push_back(glfwExtensions[i]);
    }

    // check instance extensions supported...
    if (!checkInstanceExtensionSupport(&instanceExtensions))
    {
        throw std::runtime_error ("vkInstance does not support requires extensions!");
    }
    

    createInfo.enabledExtensionCount = static_cast<uint32_t> (instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    // TODO: Set up validation layers that instance will use.
    createInfo.enabledLayerCount = 0;
    //createInfo.ppEnabledExtensionNames = nullptr;

    // Create Instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::string errorMsg = "Failed to create a Vulkan Instance. Error code: " + std::to_string(result);
        throw std::runtime_error(errorMsg);
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
    validation.setValidationLayerState();
}
