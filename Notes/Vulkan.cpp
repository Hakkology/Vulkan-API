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

-----------------

Graphics Pipeline

The process in which graphics pipeline is created is as follows:

Vertex, Index buffer => Input Assembler => Vertex Shader => Tesselation => Geometry Shader
=> Rasterization => Fragment shader => Color blending => framebuffer.

General flow -------------
Input assembler receives the input and forms the vertices.
Tesselation is used to divide shapes into lower triangles.
Geometry shader adds further geometry.
Rasterization looks at all our points and tries to figure out where all pixels should be.
Massive array of pixels are then passed to the fragment shader, where colours are processed.
Color blending adds colour and transparency.
Final stage is the framebuffer.

Data state, SPIR-V for shaders -------------
pre-compile shader code to intermediate code called SPIR-V and load it to a shader module.
(Standard Portable Intermediate Representation - Vulkan)
Compiled from GLSL => glslangValidator.exe (LunarG Vulkan SDK)
.spv file is created loaded into shader modul passed to info struct.
Put all shader into structs into list and use in Pipeline Create Info.

Fixed functions required for Graphics pipeline ----------
Vertex Input => defines layout and format of vertex input data.
Input Assembly => defines how to assemble vertices to primitives (Tris or lines).
Viewport & Scissor => how to fit output to image and crop it.
Dynamic States => pipelines are static and settings cant be changed at runtime. You need to create a pipeline to get new settings.
However some settings can be given ability to change at runtime.
Rasterizer => how to handle computation of fragments from primitives.
Multisampling => multisampling information.
Blending => how to blend fragments at the end of the pipeline.
Depth Stencil => how to determine depth, stencil culling and writing.

Pipeline layout ---------
Layout of the data being given directly to the pipeline for a single draw operation.
Defines layout of data for "Descriptor sets (uniform buffers)".
Push Constants => smaller values, similar to descriptor sets. 

Render Pass ----------------
Large operation that handles the execution and outputs of the pipeline.
Can have multiple smaller subpasses inside it that each use a different pipeline, so you can combine draws together.
Render passes have multiple attachments to all possible outputs (colour, depth).

Sub Passes -------------
Sub passes can connect to different pipelines, resulting in different graphic settings.
Sub passes rely on strict ordering to ensure data is in the right format at the right time.
Swapchain image being written to will needs to be in a writable format at the stage of subpass (logical).
However, when presented, it should be in a presentable format.
Subpass dependencies define stages in pipeline for transitions.
Implicit transitions, we only say when it should occur.
We define the layout at each stage.

Framebuffer ---------

Framebuffer is a connection between an image (or images) and the Render pass.
Attach image or multiple images to framebuffer.
Render pass outputs fragment data from a pipeline's execution, to images bound to the framebuffers attachment.
Framebuffer images line up 1-to-1 with the attachments in render pass, order should be right.

Commandbuffer --------

Vulkan works on pre-recording a group of commands and submit them all to a queue at once.
Commands will usually be in the form of:
1- Start a render pass.
2- Bind a pipeline.
3- Bind vertex/index data.
4- Bind descripter sets and push constants.
5- Draw.
A command can be submitted to begin a new subpass but a new appropriate pipeline should also be added.

Command buffers are not created like most objects in Vulkan. They are allocated from a pool.
This is a minor memory management concept:
A pool creates a space of memory partitioned into blocks of the same size, without defining their details.
Memory pools...
We can allocate one of these blocks for a command buffer.
This pool concept is useful for things that are dynamically created and destroyed frequently.

Command buffers cannot be executed directly. They need to be submitted to the appropriate queue.
In our case, we have a graphics queue so our command buffers must be performing graphical actions, such as render passes.
If the command buffer is executed, we simply submit it to a queue. The queue will constantly move and execute command buffers.
Like a conveyer belt.

Synchronization:
We should not accidentally try to access something twice.
We draw to swapchain image1 then begin to present it.
For some reason system lags and the loop returns to drawing image 1.
While its drawing, the presentation system gets over the lag and tries to display the image whilst its being drawn to.
This memory clash is unwanted.
As a fix, Semaphores and Fences are introduced.

Semaphores are flags that state if a resources can be accessed. If they are "signalled" (set to true) then the resource is available to use.
If it is "unsignalled", then the resource is in use, the program must wait.
In case of Vulkan, semaphores are used solely on the GPU itself, meaning they only create synchronization between GPU functions.
Semaphores are to indicated:
1. When an image has become available following presentation (GPU will not draw until it is available.)
2. When an image has finished being rendered and is therefore ready to present (the presentation command will await this, until rendering is complete.)

Fences are similar to semaphores, except we have the ability to unsignal a fence and wait on a fence CPU side (helps us block the CPU).
The GPU can still signal a fence to say a resource has become available. It is then up to us to unsignal when we want to use it.
vkWaitForFences: This will block the CPU code until the GPU signals the Fence.
vkResetFences: This will unsignal a fence until the gpu signals it again.
Fences are to be used to ensure a frame is available, in order to not to flood the queue with too many draw/present commands.

Loading Vertex Data ---------------
VkPipelineVErtexInputStateCreateInfo
shader binding information is passed through as this.
vertex data is represented with a buffer and a device memory.
Buffer then bound as vkCmd and executed.

Staging Buffers --------------------
Vertex Buffer:
Created but does not hold data by itself.
Describes size of data in memory with usage and queue sharing.
Holds vertex data, index data, storage data etc.
Queue sharing is used if a buffer is to be used by multiple queue families simultaneously.

Device Memory:
Raw data that holds the actual vertex data.
Memory is allocated from heap on physical device.
Ask the buffer whats needed and iterate over available memory types to find compatible type.
VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT: Memory is optimized for Device (GPU) usage, not accessible by the CPU.
VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: Memory is accessible by the CPI, map data in application to GPU.
VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: Data mapping bypass caching commands, mapped data does not need to be flushed to memory.

Mapping Memory ----------------------
Create buffer, device memory, bind two together.
vkMapMemory(...) => allows mapping of GPU memory to chosen point in Host memory.
Create arbitrary point in memory with void pointer, use vkMapMemory with memcpy.
Unmap with vkUnmapMemory.

vkCmdBindVertexBuffers binds vertex data before draw command.
vkCmdDraw with vertex count as number of vertices to draw.
If more data is needed, add another attribute to VkPipelineVertexInputStateCreateInfo.
Add new input in shader.

Index Buffer ------------------
For repeated vertices, avoid duplicated code.
Use numbers to re-reference a vertex from a list of unique vertices. Same process with creating vertex buffers.
vkCmdBindIndexBuffer instead of vertex buffer.
vkCmdDrawIndexed draws using indices and takes number of indices to draw.

Staging Buffers consists of using two kinds of memory.
One that is host visible, one that is GPU only.
Use command buffer to copy data from one another.
Host visible part of buffer+memory is staging buffer.
This would function as a Transfer Source buffer.
VK_BUFFER_USAGE_TRANSFER_SRC_BIT => source of transfer
vkMapMemory && vkUnMapMemory
VK_BUFFER_USAGE_tRANSFER_DST_BIT => destination of transfer
No need for render pass, no graphical operations.
vkCmdCopyBuffer => copy one buffer to another, takes source and destination buffers.
VkBufferCopy => struct that defines offset to copy from, offset to copy to and size of data to copy.
Command buffer is submitted to queue immediately then destroyed.
Staging buffer can also be destroyed.




*/