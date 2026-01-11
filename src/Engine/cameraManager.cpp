#include "cameraManager.h"

CameraManager::CameraManager()
    : center(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), radius(3.0f), theta(0.0f),
      phi(glm::radians(60.0f)), aspectRatio(800.0f / 600.0f), fov(20.0f),
      nearPlane(0.1f), farPlane(100.0f) {}

CameraManager::~CameraManager() {}

glm::mat4 CameraManager::getViewMatrix() const {
  float x = radius * sin(phi) * cos(theta);
  float y = radius * cos(phi);
  float z = radius * sin(phi) * sin(theta);

  glm::vec3 eye = center + glm::vec3(x, y, z);

  return glm::lookAt(eye, center, up);
}

glm::mat4 CameraManager::getProjectionMatrix() const {
  glm::mat4 projection =
      glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
  projection[1][1] *= -1; // Vulkan Y-flip
  return projection;
}

void CameraManager::updateAspectRatio(float newAspectRatio) {
  if (newAspectRatio > 0.0f) {
    aspectRatio = newAspectRatio;
  }
}

void CameraManager::rotate(float dTheta, float dPhi) {
  theta -= dTheta;
  phi -= dPhi;

  // Clamp phi to avoid flipping and Gimbal lock issues
  phi = std::clamp(phi, 0.01f, 3.14f - 0.01f);
}

void CameraManager::zoom(float dRadius) {
  radius -= dRadius;
  if (radius < 0.1f)
    radius = 0.1f;
}
