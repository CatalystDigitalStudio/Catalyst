#pragma once

#include "Windows.h"
#include "graphic/surface.h"
#include "graphic/image.h"

namespace Catalyst
{

    struct WindowsSurfaceData
    {
        HWND m_Window;
        MSG m_Message;
    };

    class WindowsSurface : public ISurface
    {

    public:
        WindowsSurface();
        ~WindowsSurface() = default;

        inline const HWND& getWindowHandle() const { return m_Data.m_Window; }

    private:
        virtual void create(SurfaceData) override;
        virtual void update() override;
        virtual void destroy() override;

        virtual ISurface* setTitle(const std::string&) override;
        virtual ISurface* setIcon(std::shared_ptr<Image>) override;
        virtual ISurface* setPosition(unsigned int x, unsigned int y) override;
        virtual ISurface* setDimension(unsigned int width, unsigned int height) override;

    private:
        SurfaceData m_WindowData;
        WindowsSurfaceData m_Data;

    };

}
