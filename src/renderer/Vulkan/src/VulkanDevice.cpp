#include "..\include\VulkanDevice.h"

VkPhysicalDeviceMemoryProperties VulkanDevice::getPhysicalMemoryProperties()
{
    //vkGetPhysicalDeviceMemoryProperties(m_pDevice, &memProperties);
    return m_PhysicalMemoryProperties;
}

uint32_t VulkanDevice::findMemoryType(uint32_t typeFilter,  VkMemoryPropertyFlags properties)
{

    for (uint32_t i = 0; i < m_PhysicalMemoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (m_PhysicalMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
}
