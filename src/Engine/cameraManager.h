#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraManager {
public:
  CameraManager();
  ~CameraManager();

  // Core camera functions
  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;

  // State modifiers
  void updateAspectRatio(float aspectRatio);
  void rotate(float dTheta, float dPhi);
  void zoom(float dRadius);

private:
  // Camera state
  glm::vec3 center;
  glm::vec3 up;

  // Orbital parameters
  float radius;
  float theta; // Azimuthal angle
  float phi;   // Polar angle

  // Projection parameters
  float aspectRatio;
  float fov;
  float nearPlane;
  float farPlane;
};
