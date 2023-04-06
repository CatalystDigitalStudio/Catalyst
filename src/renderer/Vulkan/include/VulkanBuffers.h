#pragma once

#include "vulkan/vulkan.h"
#include "VulkanDevice.h"

class VulkanBuffer
{

public:
    VulkanBuffer(VkBufferCreateInfo createInfo, VulkanDevice& device);
    ~VulkanBuffer();

    VkBuffer& getBuffer();

private:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

private:
    VulkanDevice m_Device;

private:
    VkBufferCreateInfo bufferInfo = {};
    VkMemoryAllocateInfo allocationInfo = {};

};
