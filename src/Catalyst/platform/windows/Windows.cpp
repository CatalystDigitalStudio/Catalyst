#include "pch.h"
#include "Windows.h"

#include "graphic/renderer.h"

#include "windowsSurface.h"

namespace Catalyst
{
    typedef CatalystResult(*createRnederer)(IRenderer**, RendererInfo);

    PlatformData WindowsPlatform::getPlatformData()
    {
        return data;
    }

    CatalystPtrRenderer WindowsPlatform::createRenderer(std::string path, CatalystPtrSurface surface, RendererInfo info)
    {
        CatalystPtrRenderer renderer = nullptr;

        HMODULE module = LoadLibraryA(path.c_str());
        if (!module)
        {
            Engine::raiseError({ Level::Error, "[CATALYST] [RENDERER] Failed to load renderer module!" });
            return renderer;
        }

        createRnederer fn = (createRnederer)GetProcAddress(module, "createRenderer");
        
        auto result = fn(&renderer, info);

        if (result != CatalystResult::Success)
        {
            delete renderer;
            Engine::raiseError({ Level::Error, "[CATALYST] [RENDERER] Failed to load renderer!" });
        }

        return renderer;
    }

    CatalystPtrSurface WindowsPlatform::createSurface(SurfaceData data)
    {
        CatalystPtrSurface surface = nullptr;

        surface = new WindowsSurface();
        
        surface->create(data);

        return surface;
    }

    std::shared_ptr<Platform> initalizePlatform()
    {
        return std::make_shared<WindowsPlatform>();
    }
}