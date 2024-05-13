cmake_minimum_required(VERSION 3.12)
project(Vulkan-Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Vulkan and glfw package.
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Add the executable that will be built from the source files.
add_executable(Vulkan-Renderer
    src/main.cpp
    src/Engine/deviceManager.cpp
    src/Engine/queueManager.cpp
    src/Engine/surfaceManager.cpp
    src/Engine/swapChainManager.cpp
    src/Engine/vulkanRenderer.cpp
    src/Engine/window.cpp
    src/Engine/vulkanUtils.cpp
    src/Engine/vulkanValidation.cpp
)

# Specify directories where the compiler can find headers
target_include_directories(Vulkan-Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Engine
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/glm
)

# Link the Vulkan and GLFW libraries with the executable.
target_link_libraries(Vulkan-Renderer PRIVATE Vulkan::Vulkan glfw)

message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "GLFW include dirs: ${GLFW_INCLUDE_DIRS}")
