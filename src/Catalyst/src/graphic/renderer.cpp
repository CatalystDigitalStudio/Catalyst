#include "pch.h"
#include "renderer.h"

namespace Catalyst
{
    IRenderer::IRenderer()
    {

    }
    IRenderer::~IRenderer()
    {

    }
    IDevice::IDevice(const std::string& name)
        : m_Name(name)
    {
    }
    const std::string& IDevice::getName() const
    {
        return m_Name;
    }
}

