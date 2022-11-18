#pragma once

#include "core/engine.h"
#include "graphic/renderer.h"

#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

#if defined (_WIN32)
#include "vulkan/vulkan_win32.h"
#elif defined(__linux__)
#include "vulkan/vulkan_xcb.h"
#elif defined(__ANDROID__)
#include "vulkan/vulkan_android.h"
#endif

#include <deque>

struct VulkanImage
{

    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkFormat format;

    void create(VkDevice device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

    void destroy(VkDevice device);

};

class CATALYST_API VulkanPipeline : public Catalyst::IPipeline
{

public:
    VulkanPipeline(VkDevice device, VkPhysicalDevice pDevice, VkSurfaceKHR surface);
    ~VulkanPipeline();

    VkShaderModule createModule(std::string code);
    void configurePipeline(Catalyst::CatalystShaderTopology topology, const uint32_t width, const uint32_t height);

private:
    void setupDynamicState(bool viewport, bool scissor, bool line_width, bool blend);
    void setupInput(Catalyst::CatalystShaderTopology topology);
    void setupView(const uint32_t width, const uint32_t height);
    void setupRasterizer();
    void setupMultisample();
    void setupDepth();
    void setupBlending();

private:
    virtual void initalize(Catalyst::PipelineInformation info) override;
    virtual void cleanup() override;

    virtual void addStage(Catalyst::CatalystShaderStageType stagetype, std::string byteCode) override;

    virtual std::string compileShader(std::string rawCode) override;

private:
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    VkSurfaceKHR m_Surface;

    VkViewport m_Viewport     = {};
    VkRect2D m_Scissor        = {};
    VkPipelineLayout m_Layout = {};
    VkRenderPass m_RenderPass = {};

    //std::vector<> m_Modules;

};

class CATALYST_API VulkanRenderer : public Catalyst::IRenderer
{

public:
    VulkanRenderer(Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info);
    ~VulkanRenderer();

    virtual void initalize() override;
    virtual void cleanup() override;

    virtual void createPipeline() override;
    virtual std::shared_ptr<Catalyst::IPipeline> getPipeline() override;

    VkCommandBuffer getCommandBuffer(bool begin);

private:
    void createInstance();
    void createDevice();
    void createSwapchain(Catalyst::CatalystPtrSurface);

    uint32_t scoreDeviceSuitability(VkPhysicalDevice&);

private:
    Catalyst::RendererInfo m_Info;
    VkInstance m_Instance;

    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_LogicalDevice;

    VkSurfaceKHR m_Surface;
    VkFormat m_ColorFormat;
    VkColorSpaceKHR m_ColorSpace;

    std::unordered_map<uint32_t, std::shared_ptr<VulkanPipeline>> m_Pipelines;
    std::deque<void*> m_CatalystCommandBuffer;
};
