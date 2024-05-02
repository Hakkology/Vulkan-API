#include "deviceManager.h"

DeviceManager::DeviceManager() : physicalDevice(VK_NULL_HANDLE), logicalDevice(VK_NULL_HANDLE) {
    // Constructor implementation
}

void DeviceManager::pickPhysicalDevice(VkInstance instance) {
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
        if (checkDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void DeviceManager::createLogicalDevice() {
    auto indices = VulkanUtils::findQueueFamiliesForDevice(physicalDevice);

    // Vector for queue creation information, set for family indices.
    // Since graphics and presentation queue are basically the same, we cannot have both of them available at the same time.
    // We will choose presentation if available, otherwise graphics queue will do.
    // Set makes sure that only one item is available here.
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> queueFamilyIndices = {indices.graphicsFamily, indices.presentationFamily};

    for (int queueFamilyIndex :  queueFamilyIndices)
    {
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;                                        // Index of the family to create a queue from.
        queueCreateInfo.queueCount = 1;                                                             // Number of queues to create.
        queueCreateInfo.pQueuePriorities = &queuePriority;       
        
        queueCreateInfos.push_back(queueCreateInfo);                                                // Vulkan wants to know how to handle multiple queues.
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());               // Number of queue create infos.
    createInfo.pQueueCreateInfos = queueCreateInfos.data();                                         // List of queue create infos
    //createInfo.pEnabledFeatures = &deviceFeatures;                                                  // may be deleted.
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());                                                           // Number of enabled logical device extensions.
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();                                                   // List of enabled logical device extensions.

    

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
}

VkPhysicalDevice DeviceManager::getPhysicalDevice() const {
    return physicalDevice;
}

VkDevice DeviceManager::getLogicalDevice() const {
    return logicalDevice;
}

bool DeviceManager::checkDeviceSuitable(VkPhysicalDevice device) {

    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForDevice(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    std::cout << "Checking device suitability: " << std::endl;
    std::cout << "Queue Families - Graphics: " << indices.graphicsFamily << ", Presentation: " << indices.presentationFamily << std::endl;
    std::cout << "Extensions Supported: " << extensionsSupported << std::endl;

    return indices.isValid() && extensionsSupported;
}

bool DeviceManager::checkDeviceExtensionSupport(VkPhysicalDevice device) {

    // Get Device extension count.
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    // no extension, return.
    if (extensionCount == 0)
    {
        return false;
    }
    
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

