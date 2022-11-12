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

class CATALYST_API VulkanPipeline : public Catalyst::IPipeline
{

public:
    VulkanPipeline(VkDevice device, VkSurfaceKHR surface);
    ~VulkanPipeline();

    VkShaderModule createModule(std::string code);
    void configurePipeline(Catalyst::CatalystShaderTopology topology, const uint32_t width, const uint32_t height);

private:
    virtual void initalize(Catalyst::PipelineInformation info) override;
    virtual void cleanup() override;

    virtual void addStage(Catalyst::CatalystShaderStageType stagetype, std::string byteCode) override;

    virtual std::string compileShader(std::string rawCode) override;

private:
    VkDevice m_Device;
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

    virtual void render() override;

    virtual void createPipeline() override;
    virtual std::shared_ptr<Catalyst::IPipeline> getPipeline() override;

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

    std::shared_ptr<VulkanPipeline> m_Pipeline;
};
