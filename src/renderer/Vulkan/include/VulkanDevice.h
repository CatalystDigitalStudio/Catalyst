#pragma once

#include "vulkan/vulkan.h"

class VulkanDevice
{

public:

    VkDevice& getDevice();

    VkPhysicalDevice& getPhysicalDevice();
    VkPhysicalDeviceMemoryProperties getPhysicalMemoryProperties();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    VkDevice m_Device;

    VkPhysicalDevice m_PhysicalDevice;
    VkPhysicalDeviceMemoryProperties m_PhysicalMemoryProperties;

};
