#pragma once

#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <string.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "deviceManager.h"
#include "queueManager.h"
#include "vulkanValidation.h"

class VulkanRenderer {
public:
    VulkanRenderer();

    int init(GLFWwindow* newWindow);

    ~VulkanRenderer();

    void terminate();

    // Validation functions
    void setValidationEnabled();

private:
    GLFWwindow* window;
    VkInstance instance;

    DeviceManager deviceManager;
    QueueManager queueManager;
    VulkanValidation validation;

    // Create functions
    void createInstance();

    // Support functions
    bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
};