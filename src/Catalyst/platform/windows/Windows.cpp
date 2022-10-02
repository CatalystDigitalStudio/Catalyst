#include "pch.h"
#include "Windows.h"

#include "graphic/renderer.h"

namespace Catalyst
{
    typedef IRenderer*(*createRnederer)();

    PlatformData WindowsPlatform::getPlatformData()
    {
        return data;
    }

    std::shared_ptr<IRenderer> WindowsPlatform::initalizeRenderer(std::string path)
    {
        std::shared_ptr<IRenderer> renderer;

        HMODULE module = LoadLibraryA(path.c_str());
        if (!module)
        {
            Engine::raiseError({ Level::Error, "[CATALYST] [RENDERER] Failed to load renderer module!" });
            return renderer;
        }

        createRnederer fn = (createRnederer)GetProcAddress(module, "createRenderer");
        
        renderer = std::shared_ptr<IRenderer>((fn)());

        if (!renderer)
        {
            renderer.reset();
            Engine::raiseError({ Level::Error, "[CATALYST] [RENDERER] Failed to load renderer!" });
        }

        return renderer;
    }

    std::shared_ptr<Platform> initalizePlatform()
    {
        return std::make_shared<WindowsPlatform>();
    }
}