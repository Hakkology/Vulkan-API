#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cameraManager.h"

class InputManager {
public:
  InputManager();
  ~InputManager();

  void init(GLFWwindow *window);
  void update(float deltaTime);
  void setCameraManager(CameraManager *cameraManager);

  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void cursorPositionCallback(GLFWwindow *window, double xpos,
                                     double ypos);
  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);

private:
  void handleMouseInput(double xpos, double ypos);

  GLFWwindow *window;
  CameraManager *cameraManager;

  // Mouse state
  bool isRightMouseButtonPressed;
  double lastMouseX;
  double lastMouseY;

  // Static instance for callbacks
  static InputManager *instance;
};
