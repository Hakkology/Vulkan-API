#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkanMeshDrawer.h"
#include "vulkanMeshManager.h"

#include "lightManager.h"

enum class ShapeType { TRIANGLE, CUBE, PLANE };

class GraphicsInitializer {
public:
  GraphicsInitializer(VkInstance instance, VkDevice device,
                      VkPhysicalDevice physicalDevice);
  ~GraphicsInitializer();
  void addInitialMeshes(MeshManager &meshManager,
                        ShapeType shapeType = ShapeType::TRIANGLE);
  void initLights(LightManager &lightManager);

private:
  VkDevice device;
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  MeshManager *meshManager;
  MeshDrawer *meshDrawer;
};