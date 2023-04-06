#include "VulkanRenderer.h"
#include "Catalyst.h"
#include "core/internal.h"
#include "core/application.h"
#include "graphic/renderer.h"

#ifdef _WIN32
#include "../platform/windows/windowsSurface.h"
#endif

#pragma warning(push)
#pragma warning(disable:26812)

extern "C" Catalyst::CatalystResult createVulkanRenderer(Catalyst::CatalystPtrRenderer* renderer, Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info)
{
    *renderer = new VulkanRenderer(surface, info);
    if (renderer)
    {
        return Catalyst::CatalystResult::Success;
    }
    else
    {
        return Catalyst::CatalystResult::Pointer_Is_Nullptr;
    }
}

///====================================================================================
/// VULKAN INTERNAL
///====================================================================================
/// 
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    {
        Catalyst::Engine::raiseError({
            messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ? Catalyst::Level::Error : Catalyst::Level::Error,
            pCallbackData->pMessage,
            (Catalyst::CatalystResult)pCallbackData->sType,
            __FUNCTION__
            });
        return true;
    }
    default:
    {
        //CATALYST_CORE_ERROR("Unknown error {0}, {1}", messageType, pCallbackData->pMessage);
        break;
    }
    }

    return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VulkanRenderer::VulkanRenderer(Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info)
    : m_Info(info)
{
    createInstance();

    if (surface && !(info.flags & Catalyst::CATALYST_RENDERER_FLAG_HEADLESS))
    {
        createSurface(surface);
    }

    createDevice();

    if (surface && !(info.flags & Catalyst::CATALYST_RENDERER_FLAG_HEADLESS))
    {
        createSwapchain(surface);
    }
}
VulkanRenderer::~VulkanRenderer()
{

}

void VulkanRenderer::initalize()
{

}
void VulkanRenderer::cleanup()
{
    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanRenderer::render()
{
}

const std::vector<Catalyst::IDevice*> VulkanRenderer::getDeviceList()
{
    return m_PhysicalDevices;
}

Catalyst::PipelineID VulkanRenderer::createPipeline(Catalyst::PipelineInformation information)
{
    Catalyst::PipelineID id = (Catalyst::PipelineID)m_Pipelines.size();
    m_Pipelines[id] = std::make_shared<VulkanPipeline>(m_Device, m_Surface);
    getPipeline(id)->initalize(information);
    return id;
}

std::shared_ptr<Catalyst::IPipeline> VulkanRenderer::getPipeline(Catalyst::PipelineID id)
{
    return std::static_pointer_cast<Catalyst::IPipeline>(m_Pipelines[id]);
}

constexpr Catalyst::CommandPool& VulkanRenderer::getCommandPool()
{
    return m_userCommandPool;
}

void VulkanRenderer::createInstance()
{

    VkInstanceCreateInfo createInfo = {};
    //Init
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;

    //Application

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "Catalyst Application"; ///TODO: Use user app name
    appInfo.pEngineName = "Catalyst";
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);

    createInfo.pApplicationInfo = &appInfo;

    //Extentions
    std::vector<const char*> enabledExtensions = getExtentions();
    enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    createInfo.ppEnabledExtensionNames = enabledExtensions.data();

    loadLayerList();

    //Layers
    if (m_Debug)
    {
        addLayer("VK_LAYER_KHRONOS_validation");

        createInfo.enabledLayerCount = static_cast<uint32_t>(m_UseLayers.size());
        createInfo.ppEnabledLayerNames = m_UseLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateInstance(&createInfo, NULL, &m_Instance);

    if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
        Catalyst::Engine::raiseError({ Catalyst::Level::Error,
            "[CATALYST] [VULKAN] Failed to find Vulkan driver",
            Catalyst::CatalystResult::Error,
            __FUNCTION__ });
    }
    else if (result != VK_SUCCESS) {
        Catalyst::Engine::raiseError({ Catalyst::Level::Error,
            "[CATALYST] [VULKAN] Failed to create instance, see error output",
            Catalyst::CatalystResult::Error,
            __FUNCTION__ });
    }


    if (m_Debug)
    {
        VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
        messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        messengerCreateInfo.pfnUserCallback = debugCallback;
        messengerCreateInfo.pUserData = nullptr; // Optional

        if (CreateDebugUtilsMessengerEXT(m_Instance, &messengerCreateInfo, nullptr, &m_Messenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }
}

void VulkanRenderer::loadLayerList()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    m_AvailableLayers = std::vector<VkLayerProperties>(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, m_AvailableLayers.data());
}

void VulkanRenderer::loadPhysicalDeviceList()
{
    uint32_t deviceCount = 0;
    assert(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, NULL) == VK_SUCCESS);
    assert(deviceCount >= 1);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    assert(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data()) == VK_SUCCESS);

    for (auto& device : physicalDevices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        m_PhysicalDevices.push_back(new Catalyst::IDevice(deviceProperties.deviceName));
        
    }

}

void VulkanRenderer::createDevice()
{
    m_Device = std::make_shared<VulkanDevice>();
    m_Device.reset(new VulkanDevice(m_Instance, m_Surface, {}, 1.0f, m_Debug, m_UseLayers));
}

void VulkanRenderer::createSurface(Catalyst::CatalystPtrSurface surface)
{
#if defined(_WIN32)

    Catalyst::WindowsSurface* pS = static_cast<Catalyst::WindowsSurface*>(surface);

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = (HINSTANCE)GetModuleHandleA(NULL);
    surfaceCreateInfo.hwnd = pS->getWindowHandle();
    VkResult result = vkCreateWin32SurfaceKHR(m_Instance, &surfaceCreateInfo, nullptr, &m_Surface);
#endif


    if (result != VK_SUCCESS)
        throw;
}

void VulkanRenderer::createSwapchain(Catalyst::CatalystPtrSurface surface)
{

    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->m_PhysicalDevice, m_Surface, &count, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_Device->m_PhysicalDevice, m_Surface, &count, surfaceFormats.data());

    if (count == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM; //Need to specify what format to use as the system doesn't have a prefered
        m_ColorSpace = surfaceFormats[0].colorSpace;
    }
    else
    {
        assert(count >= 1);
        m_ColorFormat = surfaceFormats[0].format;
        m_ColorSpace = surfaceFormats[0].colorSpace;
    }
}

std::vector<const char*> VulkanRenderer::getExtentions()
{
#if defined(_WIN32)
    std::vector<const char*> extentions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,

        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
#elif defined(__ANDROID__)
    std::vector<const char*> extentions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,

        VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
    };
#elif defined(__linux__)
    std::vector<const char*> extentions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,

        VK_KHR_XCB_SURFACE_EXTENSION_NAME
    };
#else
#error No supported Vulkan platform
#endif

    return extentions;
}

bool VulkanRenderer::hasLayer(const char* name)
{
    bool layerFound = false;

    for (const auto& layerProperties : m_AvailableLayers) {
        if (strcmp(name, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
        }
    }

    return layerFound;
}

void VulkanRenderer::addLayer(const char* name)
{
    if (hasLayer(name))
    {
        m_UseLayers.push_back(name);
    }
}

VkCommandBuffer VulkanRenderer::getCommandBuffer(bool begin)
{
    VkCommandBuffer cmdBuffer = {};

    VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
    cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    //cmdBufAllocateInfo.commandPool = cmdPool;
    cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufAllocateInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_Device->m_Device, &cmdBufAllocateInfo, &cmdBuffer) != VK_SUCCESS)
    {
        throw;
    }

    // If requested, also start the new command buffer
    if (begin)
    {
        VkCommandBufferBeginInfo commandBufferBegin = {};

        commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBegin.pNext = nullptr;
        commandBufferBegin.flags = 0;
        commandBufferBegin.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(cmdBuffer, &commandBufferBegin) != VK_SUCCESS)
        {
            throw;
        }
    }

    return cmdBuffer;
}

std::shared_ptr<VulkanDevice> VulkanRenderer::getDevice()
{
    return m_Device;
}

#pragma warning(pop)

VulkanPipeline::VulkanPipeline(std::shared_ptr<VulkanDevice> device, VkSurfaceKHR surface)
    : m_Device(device), m_Surface(surface)
{
}

VulkanPipeline::~VulkanPipeline()
{
}

VkShaderModule VulkanPipeline::createModule(std::string code)
{
    VkShaderModuleCreateInfo createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    createInfo.codeSize = code.length();

    VkShaderModule shaderModule = {};

    if (vkCreateShaderModule(m_Device->m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        Catalyst::Engine::raiseError({
            Catalyst::Level::Error, 
            "[CATALYST] [VULKAN] Failed to create shader module!", 
            Catalyst::CatalystResult::Error, 
            __FUNCTION__ });

        throw std::runtime_error("Failed to create shader module.");
    }
    else
    {
        return shaderModule;
    }
}

void VulkanPipeline::configurePipeline(Catalyst::CatalystShaderTopology topology, const uint32_t width, const uint32_t height)
{

    setupDynamicState(true, true, true, true);
    setupInput(topology);
    setupView(width, height);
    setupRasterizer();
    setupMultisample();
    setupDepth();
    setupBlending();


}

void VulkanPipeline::setupDynamicState(bool viewport, bool scissor, bool line_width, bool blend)
{
    std::vector<VkDynamicState> dynamicStates;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

}

void VulkanPipeline::setupInput(Catalyst::CatalystShaderTopology topology)
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo assemblyInfo = {};
    assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assemblyInfo.pNext = nullptr;
    assemblyInfo.primitiveRestartEnable = VK_FALSE;

    switch (topology)
    {
    case Catalyst::CATALYST_SHADER_TOPOLOGY_POINT_LIST:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        break;
    }

    case Catalyst::CATALYST_SHADER_TOPOLOGY_LINE_LIST:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        break;
    }
    case Catalyst::CATALYST_SHADER_TOPOLOGY_LINE_STRIP:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        break;
    }

    case Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_LIST:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        break;
    }
    case Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        break;
    }
    case Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_FAN:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        break;
    }

    default:
    {
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        throw std::runtime_error("Faulty primitive type");
    }
    }
}

void VulkanPipeline::setupView(const uint32_t width, const uint32_t height)
{
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { width, height };

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
}

void VulkanPipeline::setupRasterizer()
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
}

void VulkanPipeline::setupMultisample()
{
}

void VulkanPipeline::setupDepth()
{
}

void VulkanPipeline::setupBlending()
{
}

void VulkanPipeline::initalize(Catalyst::PipelineInformation information)
{
    configurePipeline(information.topology, information.imageWidth, information.imageHeight);
}

void VulkanPipeline::cleanup()
{
}

void VulkanPipeline::addStage(Catalyst::CatalystShaderStageType stageType, std::string byteCode)
{
    VkPipelineShaderStageCreateInfo createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pSpecializationInfo = 0;
    createInfo.pName = "main";
    createInfo.module = createModule(byteCode);

    switch (stageType)
    {
    case Catalyst::CATALYST_SHADER_STAGE_GEOMETERY:
    {
        createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; 
        break;
    }
    case Catalyst::CATALYST_SHADER_STAGE_FRAGMENT:
    {
        createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; 
        break;
    }
    default:
    {
        createInfo.stage = VK_SHADER_STAGE_ALL;
    }
    }
}

std::string VulkanPipeline::compileShader(std::string rawCode)
{
    return std::string();
}

void VulkanImage::create(VkDevice device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
    this->format = format;

    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &createInfo, nullptr, &view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image views!");
    }
}

void VulkanImage::destroy(VkDevice device)
{
    vkDestroyImageView(device, view, nullptr);
    vkDestroyImage(device, image, nullptr);
    vkFreeMemory(device, memory, nullptr);
}

VulkanDevice::VulkanDevice()
{
}

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface, const VkPhysicalDeviceFeatures& features, float queuePriority, bool debug, const std::vector<const char*>& layers, VkPhysicalDevice m_Preselection)
    : m_Instance(instance)
{

    ///=============================================================================
    ///                             PHYSICAL DEVICE
    ///=============================================================================

    if (m_Preselection == VK_NULL_HANDLE)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        ///TODO: Rating
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                m_PhysicalDevice = device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_DeviceProperties);
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_DeviceFeatures);
    }

    ///=============================================================================
    ///                                 DEVICE
    ///=============================================================================
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;


    VkDeviceQueueCreateInfo queueCreateInfo = {};

    QueueSupport queueSupport = getQueueFamiliesSupport(m_PhysicalDevice, surface);
    
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueSupport.index;
    queueCreateInfo.queueCount = 1;
                   
    queueCreateInfo.pQueuePriorities = &queuePriority;
    

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &features;

    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = {};

    if (debug) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_Device, queueCreateInfo.queueFamilyIndex, 0, &m_GraphicsQueue);

}

VulkanDevice::~VulkanDevice()
{
    if(m_Device)
        vkDestroyDevice(m_Device, nullptr);
}
bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader;
}

VulkanDevice::QueueSupport VulkanDevice::getQueueFamiliesSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueSupport support = {};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {

                support.index = i;
                support.supportsSurface = true;

                return support;
            }
        }

        i++;
    }

    return support;
}
