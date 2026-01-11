#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string.h>
#include <vector>

#include "Engine/mesh.h"
#include "Engine/meshDrawer.h"
#include "Engine/meshManager.h"
#include "Engine/scenes.h"
#include "Engine/textureManager.h"
#include "Pipeline/vulkanDepthStencilManager.h"
#include "Pipeline/vulkanShadowMapManager.h"
#include "cameraManager.h"
#include "deviceManager.h"
#include "inputManager.h"
#include "lightManager.h"
#include "queueManager.h"
#include "scene.h"
#include "surfaceManager.h"
#include "textureManager.h"
#include "vulkanCommandBuffer.h"
#include "vulkanFrameBuffer.h"
#include "vulkanGraphicsPipeline.h"
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
  std::unique_ptr<GraphicsPipeline> texturedGraphicsPipeline;
  std::unique_ptr<GraphicsPipeline> coloredMovingGraphicsPipeline;
  std::unique_ptr<GraphicsPipeline> skyboxGraphicsPipeline;
  std::unique_ptr<Renderpass> renderPass;
  std::unique_ptr<FrameManager> frameBuffer;
  std::unique_ptr<CommandManager> commandBuffer;
  std::unique_ptr<SynchronizationHandler> syncHandler;
  std::unique_ptr<VulkanDepthStencilManager> depthStencilManager;
  std::unique_ptr<VulkanShadowMapManager> shadowMapManager;

  std::unique_ptr<GraphicsPipeline> shadowPipeline;

  VkDescriptorSetLayout globalDescriptorSetLayout;
  VkDescriptorSet globalDescriptorSet;
  VkBuffer globalUboBuffer;
  VkDeviceMemory globalUboMemory;

  std::unique_ptr<MeshManager> meshManager;
  std::unique_ptr<MeshDrawer> meshDrawer;
  std::unique_ptr<SceneManager> sceneManager;
  std::unique_ptr<CameraManager> cameraManager;
  std::unique_ptr<InputManager> inputManager;
  std::unique_ptr<LightManager> lightManager;
  std::unique_ptr<TextureManager> textureManager;

  VulkanValidation validation;

  VkDescriptorSetLayout textureDescriptorSetLayout;
  VkDescriptorPool textureDescriptorPool;

  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  int currentFrame = 0;
  const int maxFramesInFlight = 2;
  bool isTerminated = false;

  // Create functions
  bool createInstance();
  bool updateSwapChainSettings();

  // Support functions
  bool
  checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions);
  static bool checkValidationLayerSupport(
      const std::vector<const char *> &validationLayers);
};