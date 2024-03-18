#pragma once

#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <string.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "utilities.h"

class VulkanRenderer {
public:
    VulkanRenderer();
    int init(GLFWwindow* newWindow);
    ~VulkanRenderer();
    void terminate();

private:
    GLFWwindow* window;
    VkInstance instance;

    struct{
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
    } mainDevice;
    
    VkQueue graphicsQueue;

    // create functions
    void createInstance();
    void createLogicalDevice();

    // get functions
    void getPhysicalDevice();

    // support functions
    // --Checker functions
    bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
    bool checkDeviceSuitable(VkPhysicalDevice device);

    // --Getter functions
    QueueFamilyIndices getQueueFamilies (VkPhysicalDevice device);
};