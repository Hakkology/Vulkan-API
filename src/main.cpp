#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <iostream>
#include <csignal>

#include "vulkanRenderer.h"
#include "window.h"

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

void signalHandler(int signum);

int main (int argc, char** argv){

    signal(SIGINT, signalHandler);
    // Create window
    Window window ("Vulkan Renderer, 800, 600");

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--enable-validation-layers") {
            vulkanRenderer.setValidationEnabled();
        }
    }

    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window.glfwWindow) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    // Loop until closed
    while (!window.shouldClose())
    {
        window.pollEvents();
        vulkanRenderer.draw();
    }

    vulkanRenderer.terminate();
    return 0;
}

void signalHandler(int signum) {
    vulkanRenderer.terminate();
    exit(signum);
}