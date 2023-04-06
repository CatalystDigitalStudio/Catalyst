#include "..\include\VulkanBuffers.h"

VulkanBuffer::VulkanBuffer(VkBufferCreateInfo bufferInfo, VulkanDevice& device)
    : m_Device(device)
{
    if (vkCreateBuffer(m_Device.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        //throw std::runtime_error("failed to create vertex buffer!");
        throw;
    }

    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(m_Device.getDevice(), buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = m_Device.findMemoryType(memoryRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(m_Device.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        //throw std::runtime_error("failed to allocate vertex buffer memory!");
        throw;
    }

    vkBindBufferMemory(m_Device.getDevice(), buffer, bufferMemory, 0);
}

VulkanBuffer::~VulkanBuffer()
{
    vkDestroyBuffer(m_Device.getDevice(), buffer, nullptr);
    vkFreeMemory(m_Device.getDevice(), bufferMemory, nullptr);
}

VkBuffer& VulkanBuffer::getBuffer()
{
    return buffer;
}
