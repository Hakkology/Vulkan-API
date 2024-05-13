#pragma once

#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <set>
#include <string.h>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

#include "deviceManager.h"
#include "queueManager.h"
#include "surfaceManager.h"
#include "vulkanValidation.h"
#include "vulkanRenderpass.h"
#include "vulkanGraphicsPipeline.h"



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
    
    std::unique_ptr<SurfaceManager> surfaceManager;
    std::unique_ptr<SwapChainManager> swapChainManager;
    std::unique_ptr<GraphicsPipeline> graphicsPipeline;
    std::unique_ptr<Renderpass> renderPass;


    VulkanValidation validation;

    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    // Create functions
    bool createInstance();
    bool updateSwapChainSettings();

    // Support functions
    bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
    bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
};