#pragma once

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "vulkanUtils.h"

class SwapChainManager {
public:
  SwapChainManager(VkPhysicalDevice device, VkSurfaceKHR surface,
                   GLFWwindow *window)
      : m_device(device), m_surface(surface), m_window(window) {};
  ~SwapChainManager();

  SwapChainDetails getSwapChainDetails(VkPhysicalDevice physicalDevice,
                                       VkSurfaceKHR surface);
  bool createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
                       VkSurfaceKHR surface, GLFWwindow *window,
                       const SwapChainDetails &details);
  void cleanupSwapChain(VkDevice device);
  VkImageView createImageView(VkDevice logicalDevice, VkImage image,
                              VkFormat format, VkImageAspectFlags aspectFlags);

  bool isSwapChainAdequate(VkPhysicalDevice physicalDevice,
                           VkSurfaceKHR surface);

  VkFormat getChosenFormat() const { return m_chosenFormat; }
  VkExtent2D &getChosenExtent() const { return *m_chosenExtent; }
  VkSwapchainKHR getSwapchain() const { return swapchain; }
  std::vector<SwapchainImage> getSwapChainImages() const {
    return swapchainImages;
  }

private:
  VkSurfaceFormatKHR
  chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
  VkPresentModeKHR chooseBestPresentationMode(
      const std::vector<VkPresentModeKHR> presentationModes);
  VkExtent2D
  chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities,
                   GLFWwindow *window);

  VkPhysicalDevice m_device;
  VkSurfaceKHR m_surface;
  GLFWwindow *m_window;
  VkSwapchainKHR swapchain;

  VkFormat m_chosenFormat;
  VkExtent2D *m_chosenExtent;
  std::vector<SwapchainImage> swapchainImages;
};
