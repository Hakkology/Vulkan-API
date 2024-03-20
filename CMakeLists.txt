cmake_minimum_required(VERSION 3.12)
project(Vulkan-Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Vulkan and glfw package.
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Specify the include directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Add the executable that will be built from the source files.
add_executable(Vulkan-Renderer
    src/main.cpp 
    src/vulkanUtils.cpp
    src/deviceManager.cpp
    src/queueManager.cpp
    src/vulkanRenderer.cpp 
    src/window.cpp
)

# Include directories for Vulkan-Renderer target
target_include_directories(Vulkan-Renderer PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${INCLUDE_DIR}  # This should include your vulkanRenderer.h file
    ${CMAKE_SOURCE_DIR}/lib/glm
)

# Link the Vulkan and GLFW libraries with the executable.
target_link_libraries(Vulkan-Renderer Vulkan::Vulkan glfw)

message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "GLFW include dirs: ${GLFW_INCLUDE_DIRS}")
