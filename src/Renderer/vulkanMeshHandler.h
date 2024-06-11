#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string.h>

#include "vulkanUtils.h"

class Mesh
{

public:
    Mesh();
    Mesh(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);
    ~Mesh();


    void destroyVertexBuffer();

    inline VkBuffer getVertexBuffer() {return vertexBuffer;}
    inline int getVertexCount() {return vertexCount;}

private:
    int vertexCount;

    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;

    void createVertexBuffer(std::vector<Vertex>* vertices);
    uint32_t findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags flags);
};


