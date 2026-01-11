// VulkanUtils.h
#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
  int graphicsFamily = -1;     // Graphics Queue Family location.
  int presentationFamily = -1; // Presentation Queue Family location.

  bool isValid() const {
    return graphicsFamily >= 0 && presentationFamily >= 0;
  }
};

struct SwapChainDetails {
  VkSurfaceCapabilitiesKHR
      surfaceCapabilities; // Surface Properties, e.g., image size/extent.
  std::vector<VkSurfaceFormatKHR>
      formats; // Surface image format, e.g., RGBA and size of each colour.
  std::vector<VkPresentModeKHR>
      presentationModes; // How images should be presented to the screen.
};

struct SwapchainImage {
  VkImage image;
  VkImageView imageView;
};

struct Vertex {
  glm::vec3 pos;    // vertex position (x, y, z)
  glm::vec3 normal; // vertex normal (x, y, z)
};

struct PushConstants {
  glm::mat4 mvp;
  glm::vec4 lightDir;
  glm::vec4 lightColor;
  glm::vec4 ambientLight;
  glm::vec4 objectColor;
};

namespace VulkanUtils {
extern QueueFamilyIndices storedIndices; // Extern declaration.

// Finds queue families without considering presentation support.
QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device);

// Finds queue families with presentation support considered.
QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device,
                                               VkSurfaceKHR surface);
} // namespace VulkanUtils
