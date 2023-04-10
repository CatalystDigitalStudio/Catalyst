#include "pch.h"
#include "renderer.h"

namespace Catalyst
{

    extern "C" std::shared_ptr<IRenderer> createVulkanRenderer(RendererInfo);
    extern "C" std::shared_ptr<IRenderer> createOpenGLRenderer(RendererInfo);

    std::shared_ptr<IRenderer> createRenderer(RendererInfo info)
    {
        std::shared_ptr<IRenderer> renderer = nullptr;
        CatalystResult result = CatalystResult::Unknown;

        switch (info.type)
        {
        case CATALYST_RENDERER_TYPE_VULKAN:
        {
            renderer = createVulkanRenderer(info);
            break;
        }
        case CATALYST_RENDERER_TYPE_OPENGL:
        {
            renderer = createOpenGLRenderer(info);
            break;
        }
        }

        if (result != CatalystResult::Success)
        {
            raiseError({ Level::Error, "[CATALYST] [RENDERER] Failed to load renderer!" });
        }

        return renderer;
    }


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

