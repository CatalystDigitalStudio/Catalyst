#include "VulkanRenderer.h"
#include "Catalyst.h"

extern "C" CATALYST_API Catalyst::IRenderer * createRenderer()
{
    return new VulkanRenderer();
}

void VulkanRenderer::initalizeSwapchain()
{
}

void VulkanRenderer::initalizeDevices()
{
    uint32_t deviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_Instance, &deviceCount, NULL) != VK_SUCCESS || !deviceCount)
    {
        CATALYST_CORE_WARN("[CATALYST] [VULKAN] No GPU's found!");
        Catalyst::Engine::raiseError({ Catalyst::Level::Warning, "Vulkan device count", Catalyst::CatalystResult::Error, __FUNCTION__ });
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    if (vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS)
    {
        CATALYST_CORE_WARN("[CATALYST] [VULKAN] Unable to list physical devices!");
        Catalyst::Engine::raiseError({ Catalyst::Level::Error, "Vulkan device list", Catalyst::CatalystResult::Error, __FUNCTION__ });
    }

    VkPhysicalDeviceProperties properties = {};
    for (auto device : physicalDevices)
    {
        memset(&properties, 0, sizeof(VkPhysicalDeviceProperties));
        vkGetPhysicalDeviceProperties(device, &properties);

        m_Devices[properties.deviceID] = std::make_shared<VulkanDevice>(device, properties);
    }

    __debugbreak();
}

void VulkanRenderer::initalize(std::shared_ptr<Catalyst::ISurface> surface)
{
    Catalyst::Logger::initalizeDefault();

    m_Surface = surface;

    VkResult result = VK_SUCCESS;
    VkInstanceCreateInfo createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = 0;
    createInfo.flags = 0;

    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledLayerCount = 0;

    createInfo.ppEnabledExtensionNames = nullptr;
    createInfo.enabledExtensionCount = 0;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = 0;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    appInfo.pEngineName = "Catalyst";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, Catalyst::Engine::getMajorVersion(), Catalyst::Engine::getMinorVersion(), Catalyst::Engine::getPatchVersion());

    appInfo.pApplicationName = "Catalyst";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0,1, 0, 0);


    createInfo.pApplicationInfo = &appInfo;

    if ((result = vkCreateInstance(&createInfo, nullptr, &m_Instance)) != VK_SUCCESS)
    {
        throw;
    }

    CATALYST_CORE_INFO("Initalized Vulkan Instance");

    initalizeDevices();
    CATALYST_CORE_INFO("Initalized Vulkan Device List");
}

void VulkanRenderer::cleanup()
{

    vkDestroyInstance(m_Instance, nullptr);

    CATALYST_CORE_INFO("Closed and cleaned Vulkan renderer");
}

void VulkanRenderer::addPipline()
{
}



VulkanDevice::VulkanDevice(VkPhysicalDevice device, VkPhysicalDeviceProperties properties)
    : m_Device(device), m_Properties(properties)
{
}
VulkanDevice::~VulkanDevice()
{

}

VkPhysicalDevice& VulkanDevice::getPhysicalDevice()
{
    return m_Device;
}
const unsigned int VulkanDevice::getID()
{
    return m_Properties.deviceID;
}
const bool VulkanDevice::getProperty(Property property)
{
    return false;
}
void VulkanDevice::setProperty(Property property, bool value)
{
    ///TODO
}
