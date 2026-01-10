#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string.h>
#include <vector>

#include "deviceManager.h"
#include "inputManager.h"
#include "queueManager.h"
#include "surfaceManager.h"
#include "vulkanCommandBuffer.h"
#include "vulkanFrameBuffer.h"
#include "vulkanGraphicsInitializer.h"
#include "vulkanGraphicsPipeline.h"
#include "vulkanMeshDrawer.h"
#include "vulkanMeshHandler.h"
#include "vulkanMeshManager.h"
#include "vulkanRenderpass.h"
#include "vulkanSyncHandler.h"
#include "vulkanValidation.h"

class VulkanRenderer {
public:
  VulkanRenderer();

  int init(GLFWwindow *newWindow);

  ~VulkanRenderer();

  void terminate();
  void draw();

  // Validation functions
  void setValidationEnabled();

private:
  GLFWwindow *window;
  VkInstance instance;

  DeviceManager deviceManager;
  QueueManager queueManager;

  std::unique_ptr<SurfaceManager> surfaceManager;
  std::unique_ptr<SwapChainManager> swapChainManager;
  std::unique_ptr<GraphicsPipeline> graphicsPipeline;
  std::unique_ptr<Renderpass> renderPass;
  std::unique_ptr<FrameManager> frameBuffer;
  std::unique_ptr<CommandManager> commandBuffer;
  std::unique_ptr<SynchronizationHandler> syncHandler;

  std::unique_ptr<MeshManager> meshManager;
  std::unique_ptr<MeshDrawer> meshDrawer;
  std::unique_ptr<GraphicsInitializer> graphics;
  std::unique_ptr<InputManager> inputManager;

  VulkanValidation validation;

  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  int currentFrame = 0;
  const int maxFramesInFlight = 2;

  // Create functions
  bool createInstance();
  bool updateSwapChainSettings();

  // Support functions
  bool
  checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions);
  static bool checkValidationLayerSupport(
      const std::vector<const char *> &validationLayers);
};