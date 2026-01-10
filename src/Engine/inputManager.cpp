#include "inputManager.h"
#include <algorithm>
#include <iostream>

InputManager *InputManager::instance = nullptr;

InputManager::InputManager()
    : window(nullptr), center(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f),
      radius(3.0f), theta(0.0f),
      phi(glm::radians(90.0f)), // Start looking at the center
      isRightMouseButtonPressed(false), lastMouseX(0.0), lastMouseY(0.0) {
  instance = this;
}

InputManager::~InputManager() {
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

void InputManager::update(float deltaTime) {
  // You can handle keyboard input here if needed
}

glm::mat4 InputManager::getViewMatrix() const {
  float x = radius * sin(phi) * cos(theta);
  float y = radius * cos(phi);
  float z = radius * sin(phi) * sin(theta);

  glm::vec3 eye = center + glm::vec3(x, y, z);

  return glm::lookAt(eye, center, up);
}

glm::mat4 InputManager::getProjectionMatrix(float aspectRatio) const {
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
  projection[1][1] *=
      -1; // Vulkan's Y coordinate is inverted compared to OpenGL
  return projection;
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
  if (instance) {
    instance->radius -= static_cast<float>(yoffset) * 0.1f;
    if (instance->radius < 0.1f)
      instance->radius = 0.1f;
  }
}

void InputManager::handleMouseInput(double xpos, double ypos) {
  double dx = xpos - lastMouseX;
  double dy = ypos - lastMouseY;

  lastMouseX = xpos;
  lastMouseY = ypos;

  float sensitivity = 0.01f;
  theta -= dx * sensitivity;
  phi -= dy * sensitivity;

  // Clamp phi to avoid flipping
  phi = std::clamp(phi, 0.01f, 3.14f - 0.01f);
}
