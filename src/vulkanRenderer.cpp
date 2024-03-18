#include "vulkanRenderer.h"  


VulkanRenderer::VulkanRenderer() {
    // Constructor implementation (if needed)
}

int VulkanRenderer::init(GLFWwindow* newWindow) {

    window = newWindow;

    try
    {
        createInstance();
        getPhysicalDevice();
        createLogicalDevice();
    }
    catch(const std::runtime_error &e)
    {
        printf("ERROR: %s\n", e.what());
        return EXIT_FAILURE;
    }
    
    return 0;
}

VulkanRenderer::~VulkanRenderer() {
    // Destructor implementation (if needed)
}

void VulkanRenderer::terminate(){
    vkDestroyDevice(mainDevice.logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::createInstance(){
    
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

void VulkanRenderer::createLogicalDevice(){

    // Get the queue family indices for the chosen pyhsical device.
    QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

    // Queue the logical device needs to create and info to do so (only 1 for now, will add more later.)
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;          //index of the family to create a queue from
    queueCreateInfo.queueCount =1;                                      //number of queueus to create
    float priority = 1.0f;
    queueCreateInfo.pQueuePriorities = &priority;                       //vulkan needs to know how to handle multiple queues, so decide on priority, 1 highest, 0 lowest.

    // information to create logical device (sometimes called device)
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;                          //number of queue createinfos
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;              //list of queue create infos so device can create required  queues.
    deviceCreateInfo.queueCreateInfoCount = 0;                          //number of enabled logical device extensions.
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;                 //list of enabled logical device extensions.
    deviceCreateInfo.enabledLayerCount = 0;

    // physical device features the logical device will be using.
    VkPhysicalDeviceFeatures deviceFeatures ={};
    deviceCreateInfo.pEnabledFeatures= &deviceFeatures;                 //physical device features logical device will use.

    // create a logical devices for the given physical device.
    VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to initialize logical device.");
    }
    
    // queues are created at the same time as the device.
    // so we need to handlet he queues.
    // from given logical device, of given queue family, of given queue index, 0 since onlyo one queue, place references in given queue.
    vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
}

void VulkanRenderer::getPhysicalDevice(){

    // Enumerate physical devices the vkinstance can access
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // If no devices available, then none support Vulkan!
    if (deviceCount == 0)
    {
        throw std::runtime_error("Can't find GPU's that support Vulkan Instance!");
    }

    // Get list of physical devices
    std::vector <VkPhysicalDevice> deviceList(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

    for (const auto &device : deviceList)
    {
        if (checkDeviceSuitable(device))
        {
            mainDevice.physicalDevice = device;
            break;
        }
           
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

bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
    /*
    
        // Information about device itself (ID, name, type, vendor, etc)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        // Information about what the device can do (geo shader, tess shader, wide lines etc)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    */

    QueueFamilyIndices indices = getQueueFamilies(device);

    return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    // Get all queue family property info for the given device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    // Go through each queue family and check if it has at least 1 of the required types of queue.
    int i = 0;
    for (const auto &queueFamily : queueFamilyList)
    {
        // first check if queue family has at least 1 queue in that family, could have no queues.
        // queue can be multilpe types defined through bitfield. need to bitwise and with vk_queue_*_bit to check if it has required bits.
        if (queueFamily.queueCount >0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;         // if queue family is valid, then get index.
        }
        
        // check if queue family indices are in a valid state, stop searching if so.
        if (indices.isValid())
        {
            break;
        }
        
        i++;
    }
    
    return indices;
}
