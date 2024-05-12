#include "deviceManager.h"

DeviceManager::DeviceManager() : physicalDevice(VK_NULL_HANDLE), logicalDevice(VK_NULL_HANDLE) {
    // Constructor implementation
}

void DeviceManager::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, GLFWwindow* window) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    // Populate the devices vector
    VkResult result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to enumerate physical devices properly.");
    }

    // Iterate over all devices to find the first suitable one
    for (const auto& device : devices) {
        if (checkDeviceSuitable(device, surface, window)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void DeviceManager::createLogicalDevice(VkSurfaceKHR surface) {
    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForSurface(physicalDevice, surface);

    std::cout << "Using Graphics Family Index: " << indices.graphicsFamily << std::endl;
    std::cout << "Using Presentation Family Index: " << indices.presentationFamily << std::endl;

    // Collect unique queue family indices that are valid
    std::set<int> uniqueQueueFamilies;
    if (indices.graphicsFamily != -1) {
        uniqueQueueFamilies.insert(indices.graphicsFamily);
    }
    if (indices.presentationFamily != -1) {
        uniqueQueueFamilies.insert(indices.presentationFamily);
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;                                         // Index of the family to create a queue from.
        queueCreateInfo.queueCount = 1;                                                         // Number of queues to create
        queueCreateInfo.pQueuePriorities = &queuePriority;                                      
        queueCreateInfos.push_back(queueCreateInfo);                                            // Vulkan needs to know how to handle multiple queues.

        std::cout << "Queue Create Info added for family index: " << queueFamily << std::endl;
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());           // Number of queue create infos.
    createInfo.pQueueCreateInfos = queueCreateInfos.data();                                     // List of queue create infos.
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());          // Number of enabled logical devices.
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();                               // List of enabled logical device extensions.

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    } else {
        std::cout << "Logical device created successfully." << std::endl;
    }
}

VkPhysicalDevice DeviceManager::getPhysicalDevice() const {
    return physicalDevice;
}

VkDevice DeviceManager::getLogicalDevice() const {
    return logicalDevice;
}

bool DeviceManager::checkDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window) {
    // Retrieve queue family indices that are necessary for device suitability
    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForSurface(device, surface);

    // Check for the support of necessary device extensions
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    // Create an instance of SwapChainManager and retrieve swap chain details
    SwapChainManager swapChainManager(device, surface, window);
    SwapChainDetails swapChainDetails = swapChainManager.getSwapChainDetails(device, surface);

    // Check swap chain adequacy
    bool swapChainAdequate = !swapChainDetails.formats.empty() && !swapChainDetails.presentationModes.empty();

    std::cout << "Checking device suitability: " << std::endl;
    std::cout << "Queue Families - Graphics: " << indices.graphicsFamily;
    std::cout << ", Presentation: " << indices.presentationFamily << std::endl;
    std::cout << "Extensions Supported: " << (extensionsSupported ? "Yes" : "No") << std::endl;
    std::cout << "Swap Chain Adequate: " << (swapChainAdequate ? "Yes" : "No") << std::endl;

    // Determine if the device is suitable
    return indices.isValid() && extensionsSupported && swapChainAdequate;
}

bool DeviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {

    // Get Device extension count.
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    // no extension, return.
    if (extensionCount == 0) return false;

    // Populate list of extensions.
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

    for(const auto &deviceExtension : deviceExtensions)
    {
        bool hasExtension = false;
        for(const auto &extension : extensions)
        {
            if (strcmp(deviceExtension, extension.extensionName) == 0)
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

