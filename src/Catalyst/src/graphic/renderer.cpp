#include "pch.h"
#include "renderer.h"

namespace Catalyst
{
    const std::unordered_map<DeviceID, device_ptr> IRenderer::enumerateDevices()
    {
        return m_Devices;
    }
    const std::shared_ptr<IDevice> IRenderer::getDevices(DeviceID id)
    {
        return (m_Devices.find(id))->second;
    }
    IRenderer::IRenderer()
    {

    }
    IRenderer::~IRenderer()
    {
    }
}

