#pragma once

#include "graphic/renderer.h"

#include "vulkan/vulkan.h"


class CATALYST_API VulkanDevice : public Catalyst::IDevice
{

public:
    VulkanDevice(VkPhysicalDevice device, VkPhysicalDeviceProperties properties);
    ~VulkanDevice();

    VkPhysicalDevice& getPhysicalDevice();

private:
    virtual const unsigned int getID() override;
    virtual const bool getProperty(Property) override;
    virtual void setProperty(Property, bool value) override;

private:
    VkPhysicalDevice m_Device;
    VkPhysicalDeviceProperties m_Properties;
};


class CATALYST_API VulkanRenderer : public Catalyst::IRenderer
{

public:
    VulkanRenderer() = default;
    ~VulkanRenderer() = default;


private:
    virtual void initalize(std::shared_ptr<Catalyst::ISurface>) override;
    virtual void cleanup() override;

    virtual void addPipline() override;

    
private: //Internal class maintenence methods
    virtual void initalizeSwapchain() override;
    virtual void initalizeDevices() override;

private:
    VkInstance m_Instance = {};
};
