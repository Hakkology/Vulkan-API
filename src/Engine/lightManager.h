#pragma once

#include <glm/glm.hpp>
#include <vector>

struct DirectionalLight {
  glm::vec4 direction; // w is ignored/padding
  glm::vec4 color;     // w can be intensity
};

struct PointLight {
  glm::vec3 position;
  float constant;
  glm::vec3 color;
  float linear;
  float quadratic;
  float padding[3]; // Align to 16 bytes
};

struct SpotLight {
  glm::vec3 position;
  float cutOff;
  glm::vec3 direction;
  float outerCutOff;
  glm::vec3 color;
  float constant;
  float linear;
  float quadratic;
};

class LightManager {
public:
  LightManager();
  ~LightManager();

  void setDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color,
                           float intensity = 1.0f);
  DirectionalLight getDirectionalLight() const;

private:
  DirectionalLight directionalLight;
};
