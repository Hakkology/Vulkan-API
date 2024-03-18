#include "window.h"

Window::Window(std::string wName, const int width, const int height) {
    initWindow(wName, width, height);
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void Window::initWindow(std::string wName, const int width, const int height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindow = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(glfwWindow);
}

void Window::pollEvents() {
    glfwPollEvents();
}
