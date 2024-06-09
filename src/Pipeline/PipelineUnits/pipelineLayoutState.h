#include <vulkan/vulkan.h>
#include <iostream>

class PipelineLayout {
public:
    PipelineLayout(VkDevice device);
    ~PipelineLayout();

    const VkPipelineLayoutCreateInfo* getLayoutInfo() const;
    VkPipelineLayout getPipelineLayout() const;
    bool createPipelineLayout();

private:
    VkDevice device;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipelineLayoutCreateInfo layoutInfo;

    void setupLayoutInfo();
};