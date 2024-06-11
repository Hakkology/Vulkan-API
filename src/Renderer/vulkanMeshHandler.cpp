#include "vulkanMeshHandler.h"


Mesh::Mesh(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices) 
: physicalDevice(physicalDevice), device(device)
{
    vertexCount = vertices-> size();
    createVertexBuffer(vertices);
}

Mesh::~Mesh()
{
    destroyVertexBuffer();
}

void Mesh::destroyVertexBuffer()
{
    if (vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vertexBuffer = VK_NULL_HANDLE;
        std::cout << "Vertex Buffer destroyed." << std::endl;
    }

    if (vertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, vertexBufferMemory, nullptr);
        vertexBufferMemory = VK_NULL_HANDLE;
        std::cout << "Vertex Buffer Memory freed." << std::endl;
    }
}

void Mesh::createVertexBuffer(std::vector<Vertex> *vertices)
{
    // Create vertex buffer
    // Information to create a buffer (doesn't include assigning memory)
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Vertex) * vertices->size();        // size of buffer (size of 1 vertex * number of vertices)
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;       // multiple types of buffer possible, we want vertex buffer.
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;         // Similar to swap chain images, can share vertex buffers.

    VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a vertex buffer!");
    }
    
    // Get buffer memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    // Allocate memory to buffer
    VkMemoryAllocateInfo memoryAllocInfo = {};
    memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocInfo.allocationSize = memRequirements.size;                                              // index of memory type on physical device that has required bit flags.
    memoryAllocInfo.memoryTypeIndex = findMemoryTypeIndex(memRequirements.memoryTypeBits,               // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : CPU can interact with memory.
                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); 
                                        // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : allows placement of data straight into buffer after mapping.
    
    // Allocate memory to VkDeviceMemory
    result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, &vertexBufferMemory);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocated vertex buffer memory!");
    }

    // Allocate memory to given vertex buffer
    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);

    // Map memory to vertex buffer.
    void *data;                                                             // 1. Create pointer to a point in normal memory.
    vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);  // 2. Map the vertex buffer memory to that point.
    memcpy(data, vertices->data(), (size_t)bufferInfo.size);                // 3. Copy memory from vertices vector to the point.
    vkUnmapMemory(device, vertexBufferMemory);                              // 4. Unmap vertex buffer memory.
}

uint32_t Mesh::findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
    // Get properties of physical device memory
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((allowedTypes & (1 << i))                               // Index of memory type must match corresponding bit in allowed Types
        && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) // desired property bit flags are part of memory flags.
        {
            return i;
        }
    }
    
}
