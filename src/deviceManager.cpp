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
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Just pick the first device for now
    physicalDevice = devices[0];

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void DeviceManager::createLogicalDevice() {
    auto indices = VulkanUtils::findQueueFamilies(physicalDevice);

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
    createInfo.pEnabledFeatures = &deviceFeatures;                                                  // may be deleted.
    createInfo.enabledExtensionCount = 0;                                                           // Number of enabled logical device extensions.
    createInfo.ppEnabledExtensionNames = nullptr;                                                   // List of enabled logical device extensions.

    

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

bool DeviceManager::checkDeviceSuitable(VkPhysicalDevice device, const QueueFamilyIndices &indices) {
    // No need to findQueueFamilies here as indices are passed in
    return indices.isValid();
}

