#include "inputManager.h"
#include <algorithm>
#include <iostream>

InputManager *InputManager::instance = nullptr;

InputManager::InputManager()
    : window(nullptr), cameraManager(nullptr), isRightMouseButtonPressed(false),
      lastMouseX(0.0), lastMouseY(0.0) {
  instance = this;
}

InputManager::~InputManager() {
  if (window) {
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
  }
  if (instance == this) {
    instance = nullptr;
  }
}

void InputManager::init(GLFWwindow *window) {
  this->window = window;
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, cursorPositionCallback);
  glfwSetScrollCallback(window, scrollCallback);
}

void InputManager::setCameraManager(CameraManager *cameraManager) {
  this->cameraManager = cameraManager;
}

void InputManager::update(float deltaTime) {
  // You can handle keyboard input here if needed
}

void InputManager::mouseButtonCallback(GLFWwindow *window, int button,
                                       int action, int mods) {
  if (instance) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (action == GLFW_PRESS) {
        instance->isRightMouseButtonPressed = true;
        glfwGetCursorPos(window, &instance->lastMouseX, &instance->lastMouseY);
      } else if (action == GLFW_RELEASE) {
        instance->isRightMouseButtonPressed = false;
      }
    }
  }
}

void InputManager::cursorPositionCallback(GLFWwindow *window, double xpos,
                                          double ypos) {
  if (instance && instance->isRightMouseButtonPressed) {
    instance->handleMouseInput(xpos, ypos);
  }
}

void InputManager::scrollCallback(GLFWwindow *window, double xoffset,
                                  double yoffset) {
  if (instance && instance->cameraManager) {
    instance->cameraManager->zoom(static_cast<float>(yoffset) * 0.1f);
  }
}

void InputManager::handleMouseInput(double xpos, double ypos) {
  if (!cameraManager)
    return;

  double dx = xpos - lastMouseX;
  double dy = ypos - lastMouseY;

  lastMouseX = xpos;
  lastMouseY = ypos;

  float sensitivity = 0.01f;
  cameraManager->rotate(dx * sensitivity, dy * sensitivity);
}
