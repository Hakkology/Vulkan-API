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

*/