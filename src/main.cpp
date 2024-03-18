#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <iostream>

#include "vulkanRenderer.h"
#include "window.h"

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

int main (){

    // Create window
    Window window ("Vulkan Renderer, 800, 600");
    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window.glfwWindow) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    // Loop until closed
    while (!window.shouldClose())
    {
        window.pollEvents();
    }

    vulkanRenderer.terminate();

    return 0;

}