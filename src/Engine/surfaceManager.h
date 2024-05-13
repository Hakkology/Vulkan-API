#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class SurfaceManager {
public:
    SurfaceManager(VkInstance instance, GLFWwindow* window);
    ~SurfaceManager();

    VkSurfaceKHR getSurface() const;

private:
    VkInstance instance;
    VkSurfaceKHR surface;
    GLFWwindow* window;

    void initSurface();
};