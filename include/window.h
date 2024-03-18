#pragma once

#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    GLFWwindow* glfwWindow;

    Window(std::string wName = "Vulkan Renderer", const int width = 800, const int height = 600);
    ~Window();

    bool shouldClose();
    void pollEvents();

private:
    void initWindow(std::string wName, const int width, const int height);
};
