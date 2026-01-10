#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class InputManager {
public:
  InputManager();
  ~InputManager();

  void init(GLFWwindow *window);
  void update(float deltaTime);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix(float aspectRatio) const;

  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void cursorPositionCallback(GLFWwindow *window, double xpos,
                                     double ypos);
  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);

private:
  void handleMouseInput(double xpos, double ypos);

  GLFWwindow *window;

  // Camera parameters
  glm::vec3 center;
  glm::vec3 up;
  float radius;
  float theta; // Azimuthal angle
  float phi;   // Polar angle

  // Mouse state
  bool isRightMouseButtonPressed;
  double lastMouseX;
  double lastMouseY;

  // Static instance for callbacks
  static InputManager *instance;
};
