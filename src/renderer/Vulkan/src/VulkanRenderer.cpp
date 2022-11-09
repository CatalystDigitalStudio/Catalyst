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

extern "C" CATALYST_API Catalyst::CatalystResult createRenderer(Catalyst::CatalystPtrRenderer renderer, Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info)
{
    renderer = new VulkanRenderer(surface, info);
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

VulkanRenderer::VulkanRenderer(Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info)
    : m_Info(info)
{
    createInstance();
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
    vkDestroyDevice(m_LogicalDevice, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanRenderer::createPipeline()
{
    m_Pipeline = std::make_shared<VulkanPipeline>(m_LogicalDevice, m_Surface);
}

std::shared_ptr<Catalyst::IPipeline> VulkanRenderer::getPipeline()
{
    return std::static_pointer_cast<Catalyst::IPipeline>(m_Pipeline);
}

void VulkanRenderer::createInstance()
{

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = Catalyst::IApplication::get()->name();
    appInfo.pEngineName = "Catalyst";
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

    if (!(m_Info.flags & Catalyst::CATALYST_RENDERER_FLAG_HEADLESS))
#if defined(_WIN32)
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__ANDROID__)
    enabledExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
    enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    createInfo.ppEnabledExtensionNames = enabledExtensions.data();

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
}

void VulkanRenderer::createDevice()
{
    uint32_t deviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, NULL);
    assert(result == VK_SUCCESS);
    assert(deviceCount >= 1);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);

    result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount,
        physicalDevices.data());
    assert(result == VK_SUCCESS);

    m_PhysicalDevice = physicalDevices[0];

    for (uint32_t i = 0, top = 0; i < deviceCount; i++) 
    {
        uint32_t score = scoreDeviceSuitability(physicalDevices[i]);

        if (score > top)
        {
            m_PhysicalDevice = physicalDevices[i];
            top = score;
        }
    }

    float priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.flags = 0;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priorities[0];

    std::vector<const char*> enabledExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = enabledExtensions.size();
    deviceInfo.ppEnabledExtensionNames = enabledExtensions.data();
    deviceInfo.pEnabledFeatures = NULL;

    result = vkCreateDevice(m_PhysicalDevice, &deviceInfo, NULL, &m_LogicalDevice);
    assert(result == VK_SUCCESS);
}

void VulkanRenderer::createSwapchain(Catalyst::CatalystPtrSurface surface)
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

    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &count, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &count, surfaceFormats.data());

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

uint32_t VulkanRenderer::scoreDeviceSuitability(VkPhysicalDevice& physicalDevice)
{
    uint32_t score = 0;
    uint32_t queueCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> familyProperties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, familyProperties.data());

    for (auto& queueProperties : familyProperties)
    {
        score += queueProperties.queueCount;
        if (queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            ++score;
        }
    }

    return score;
}

#pragma warning(pop)

VulkanPipeline::VulkanPipeline(VkDevice device, VkSurfaceKHR surface)
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

    if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
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
