/*

To do list:

- Initiate Vulkan and setup GPU for usage.
- Validation layers to validate Vulkan code.
- Setup and use of Swapchain (swap buffer functionality)
- Create pipeline and shaders in SPIR-V
- Setup a Render Pass and Sub passes
- Create and use command buffer on queue
- Load in vertex and index data (optimization)
- Descriptor sets and Push constants
- Setting up a depth buffer.
- Texture Implementation.
- Multiple subpasses with input attachments.

-------------------------

Extension detailing:

VK_KHR_device_group_creation: This extension provides functionality for creating devices in a device group for multi-GPU rendering.
VK_KHR_external_fence_capabilities, VK_KHR_external_memory_capabilities, VK_KHR_external_semaphore_capabilities: These extensions provide capabilities for handling synchronization objects that can be shared across process boundaries or between Vulkan and other APIs.
VK_KHR_get_physical_device_properties2, VK_KHR_get_surface_capabilities2: These extensions provide enhanced methods for querying device and surface properties.
VK_KHR_surface, VK_KHR_wayland_surface, VK_KHR_xcb_surface, VK_KHR_xlib_surface: These extensions provide support for creating Vulkan surfaces on different window systems (e.g., XCB, Xlib, Wayland).
VK_EXT_debug_report, VK_EXT_debug_utils: These extensions provide debugging and validation functionality to help developers identify and diagnose issues with their Vulkan applications.
VK_KHR_display, VK_KHR_get_display_properties2, VK_EXT_acquire_xlib_display, VK_EXT_direct_mode_display, VK_EXT_display_surface_counter, VK_EXT_acquire_drm_display: These extensions provide support for Vulkan rendering to displays and querying display properties.
VK_EXT_swapchain_colorspace: This extension provides support for specifying the color space of swapchain images.
VK_EXT_surface_maintenance1: This extension provides maintenance functionality for Vulkan surfaces.
VK_KHR_portability_enumeration: This extension provides a mechanism for querying whether an object created by one Vulkan implementation can be used with another implementation.
VK_LUNARG_direct_driver_loading: This extension provides direct driver loading support.

Swapchain modes:
VK_PRESENT_MODE_IMMEDIATE_KHR => Surface image will be replaced immediately after the next image.
VK_PRESENT_MODE_MAILBOX_KHR => Images ready to present are added to a queue of size 1 as tripple buffer.
VK_PRESENT_MODE_FIFO_KHR => Images are added to a queue of certain size. First in first out principle.
VK_PRESENT_MODE_FIFO_RELAXEd_KHR => As soon as queue is empty, it will start acting like immediate mode. 

-------------------------

    Vulkan Instance - Vulkan context

    Define Vulkan version and capabilities.
    All Vulkan apps create Vulkan Instance.
    Enumerate all the physical devices.
    Instance creates a logical device to handle the work.

    Devices:

    -   Physical Device
    -   Logical Device

    Physical Device is GPU itself. Holds memory and queues to process pipeline, cant be interacted directly.
    Logical Device is an interface to the physical device. To be used often, set things up on the GPU.

    Physical Device -----------
    Memory: When we want to allocate memory to resources.
    Queues: Process commands submitted to GPU in FIFO order. Different queues can be used for different types of command.
    FIFO Order: First in, first out. It means that commands are processed in the order they are submitted.
    Physical devices cannot be created but they are retrieved. Enumerate over all the devices and pick a suitable one.

    Physical devices can have multiple types of queue.
    Types are referred to as queue families.
    A family can have more than one queue.
        Graphics: a family for processing graphics commands.
        Compute: a family for processing compute shaders (generic commands).
        Transfer: a family for data transfer operations.
    Queue families can be a combination of these.
    When enumerating, device should have the family queues required for the application.

    Logical Device --------------
    Acts as an interface to the physical device.
    Referenced a lot in Vulkan functions.
    Most Vulkan objects are created on the device, we use ref to the logical device to state which device to create objects on.
    Creation steps:
        -   Define queue familes and number of queues you wish to assign to the logical device from the physical device.
        -   Define all the device features you wish to enable (e.g geometry shader, anisotropy, wide lines, etc.)
        -   Define extensions the device will use.
        -   No more validation layers.

    Extensions ----------------
    Vulkan is cross platform and windows are defined differently for different systems.
    Vulkan uses extensions to add window functionality.
    Extensions are mostly commonly used that they come pre-packaged with Vulkan.
    Extensions can be chosen manually, but GLFW library chooses one for us in this case.

    GLFW ----------------
    Graphics Library framework, originally designed for OpenGL and updated to work with Vulkan.
    ALlows cross-platform window creation and automatic interfacing with OpenGL/Vulkan.
    Contains function that identifies Vulkan extensions for the host system and returns a list of them.
    glfwGetRequiredInstanceExtensions();
    Can use this list to set up Vulkan with the correct extensions.

    Validation Layers ------------
    Vulkan does not validate code, needs validation, otherwise only crashes.
    This is to avoid unnecessary overhead of error checking in release code.
    Must enable validation layer to check.
    Each layer can check different functions.

    VK_LAYER_LUNARG - validates swapchain func. Layers are similar to extensions but not in core Vulkan code. Must be acquired from third parties. LunarG has some validation layers.
    Reporting validation error should also require another extension. 
    Validation layers are now inserted into logical devices.

    DoubleBuffer -------------
    Swapchain: Complex Object that handles the retrieval and updating of images being displayed, or yet to be displayed.
    Surface: An interface between the window and the image on the swapchain between GLFW and Vulkan defined image on Swapchain.
    glfwCreateWindowSurface
    Presentation queue will help us swap our rendered image to the surface.
    Swapchain is a group of images prepared for display.

    - Surface Capabilities - image size.
    - Surface Format - image format.
    - Presentation Mode - The order and timing of images.
        - 4 presentation modes, 2 of them obsolete due to tearing.
        - At any one time, there will be one image drawn to the screen.

    ImageView shall act as an interface to draw the image.



    

    



*/