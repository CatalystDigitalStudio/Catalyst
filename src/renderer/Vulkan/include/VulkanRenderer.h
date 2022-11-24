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

class CATALYST_API VulkanDevice
{

    struct QueueSupport
    {
        uint32_t index = -1;
        bool supportsSurface = false;
    };

public:
    VulkanDevice();
    VulkanDevice(VkInstance instance, const VkPhysicalDeviceFeatures& features, float queuePriority, bool debug, const std::vector<const char*>& layers, VkPhysicalDevice m_Preselection = VK_NULL_HANDLE);
    ~VulkanDevice();

private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueSupport getQueueFamiliesSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

public:
    VkInstance      m_Instance = VK_NULL_HANDLE;

    VkPhysicalDevice           m_PhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties m_DeviceProperties;
    VkPhysicalDeviceFeatures   m_DeviceFeatures;

    VkDevice                             m_Device = VK_NULL_HANDLE;

    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue = VK_NULL_HANDLE;
};

class CATALYST_API VulkanPipeline : public Catalyst::IPipeline
{

public:
    VulkanPipeline(std::shared_ptr<VulkanDevice> device, VkSurfaceKHR surface);
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
    std::shared_ptr<VulkanDevice> m_Device;
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
    ///==========================================================================================
    ///  VulkanRenderer
    ///==========================================================================================
    
    /**
     * Initalization.
     * 
     * \param surface - Surface for swapchain, can be null if flag 'CATALYST_RENDERER_FLAG_HEADLESS' is used.
     * \param info - Initalization info.
     */
    VulkanRenderer(Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info);
    ~VulkanRenderer();


    ///==========================================================================================
    ///  Overloads
    ///==========================================================================================
    
    virtual void initalize() override;
    virtual void cleanup() override;

    virtual void render() override;


    virtual const std::vector<Catalyst::IDevice*> getDeviceList() override;

    ///==========================================================================================
    ///  Pipeline
    ///==========================================================================================
    
    virtual Catalyst::PipelineID createPipeline(Catalyst::PipelineInformation) override;
    virtual std::shared_ptr<Catalyst::IPipeline> getPipeline(Catalyst::PipelineID) override;

    ///==========================================================================================
    ///  Command Pool
    ///==========================================================================================

    virtual constexpr Catalyst::CommandPool& getCommandPool() override;

    VkCommandBuffer getCommandBuffer(bool begin);

private:
    ///==========================================================================================
    ///  Setup/Creation/Initalization Functions
    ///==========================================================================================
    
    void createInstance();
    void createDevice();
    void createSwapchain(Catalyst::CatalystPtrSurface);

    void loadLayerList();
    void loadPhysicalDeviceList();

    ///==========================================================================================
    ///  Utility Functions
    ///==========================================================================================
    
    std::vector<const char*> getExtentions();

    bool hasLayer(const char* name);
    void addLayer(const char* name);

    uint32_t scoreDeviceSuitability(VkPhysicalDevice&);

private:
    ///==========================================================================================
    ///  Catalyst Variables
    ///==========================================================================================
    Catalyst::RendererInfo m_Info;
    std::unordered_map<Catalyst::PipelineID, std::shared_ptr<VulkanPipeline>> m_Pipelines;
    Catalyst::CommandPool m_userCommandPool;
    std::vector<const char*> m_UseLayers;

    ///==========================================================================================
    ///  Vulkan Variables
    ///==========================================================================================

    // Buffers

    std::vector<VkLayerProperties> m_AvailableLayers; 
    std::vector<Catalyst::IDevice*> m_PhysicalDevices;

    // Instance

    VkInstance m_Instance;
    bool m_Debug = true;
    VkDebugUtilsMessengerEXT m_Messenger;

    //Devices
    std::shared_ptr<VulkanDevice> m_Device;

    //Swapchain

    VkSurfaceKHR m_Surface;
    VkFormat m_ColorFormat;
    VkColorSpaceKHR m_ColorSpace;
};


/*
 * Instance +               Models ----+--- Textetures
 *          |                          |
 *       Device ------> Memory --------+--------------+--- Memory Barriers
 *          |            |                            |
 *       Queues  +-- CommandPool   Descriptor Pool  Buffers
 *          |    |       |                |           |
 *          +    |       |                |          View ---------------------------+
 *          |    |       |                |           |                              |
 *       Family--+       |                +-- Descriptor Set / Update Current Set    |
 *                   Command Buffer       |                                          |
 *                       |                |                                          |
 *                       +----------------+------------------------------------ Framebuffers
 *                       |
 *                    Render()
 *               (Populate cmd buffer, Setup Sync, Render)
 *
 * NOTE This doesn't include the pipeline, constants, or layouts.
 *
*/