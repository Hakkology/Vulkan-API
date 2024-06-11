#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkanMeshDrawer.h"
#include "vulkanMeshManager.h"

class GraphicsInitializer {
public:
    GraphicsInitializer(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice);
    ~GraphicsInitializer();
    void addInitialMeshes(MeshManager& meshManager);

private:
    VkDevice device;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    MeshManager* meshManager;
    MeshDrawer* meshDrawer;
};