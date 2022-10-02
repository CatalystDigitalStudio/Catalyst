#include "pch.h"
#include "windowsSurface.h"
#include "graphic/surface.h"
#include "core/application.h"

#define CLASS_NAME "Catalyst_Window_Class"

static LRESULT windowProcedure(HWND hWnd, UINT Msg, WPARAM wP, LPARAM lP)
{
    switch (Msg)
    {
    case WM_DESTROY: //Window 'X' is pressed
    {
        Catalyst::IApplication::get()->close(true);
        break;
    }
    default:
        break;
    }

    return DefWindowProcA(hWnd, Msg, wP, lP);
}

namespace Catalyst
{

    WindowsSurface::WindowsSurface()
    {
        m_Data = new SurfaceData();
    }

    void WindowsSurface::create()
    {
        WNDCLASSEXA w_Class = {};

        w_Class.cbSize = sizeof(WNDCLASSEXA);

        w_Class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
        w_Class.hInstance = Engine::getPlatform()->getPlatformData().instance;
        w_Class.lpfnWndProc = &windowProcedure;
        w_Class.lpszClassName = CLASS_NAME;
        w_Class.lpszMenuName = NULL;
        w_Class.hCursor = NULL;
        w_Class.hIcon = NULL;
        w_Class.hIconSm = NULL;
        w_Class.cbWndExtra = NULL;
        w_Class.cbClsExtra = NULL;

        if (!RegisterClassExA(&w_Class))
        {
            Engine::raiseError({ Level::Error, "[CATALYST] [WIN32 ERROR]", (CatalystResult)GetLastError(), __FUNCTION__});
            return;
        }

        m_Data->m_Window = CreateWindowExA(0, CLASS_NAME, m_Title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Engine::getPlatform()->getPlatformData().instance, nullptr);

        if (!m_Data->m_Window)
        {
            Engine::raiseError({ Level::Error, "[CATALYST][WIN32 ERROR]", (CatalystResult)GetLastError(), __FUNCTION__ });
            return;
        }

        ShowWindow(m_Data->m_Window, SHOW_OPENWINDOW);
        UpdateWindow(m_Data->m_Window);
    }
    void WindowsSurface::update()
    {
        while (PeekMessageA(&m_Data->m_Message, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&m_Data->m_Message);
            DispatchMessageA(&m_Data->m_Message);
        }
    }
    void WindowsSurface::destroy()
    {
        if (m_Data->m_Window)
        {
            DestroyWindow(m_Data->m_Window);
        }

        UnregisterClassA(CLASS_NAME, Engine::getPlatform()->getPlatformData().instance);
    }


    ISurface* WindowsSurface::setTitle(const std::string& title)
    {

        SetWindowTextA(m_Data->m_Window, title.c_str());

        return this;
    }
    ISurface* WindowsSurface::setIcon(std::shared_ptr<Image> image)
    {
        return this;
    }
    ISurface* WindowsSurface::setPosition(unsigned int x, unsigned int y)
    {
        SetWindowPos(m_Data->m_Window, NULL, (m_X = x), (m_Y = y), m_Width, m_Height, 0);

        return this;
    }
    ISurface* WindowsSurface::setDimension(unsigned int width, unsigned int height)
    {
        SetWindowPos(m_Data->m_Window, NULL, m_X, m_Y, (m_Width = width), (m_Height = height), 0);

        return this;
    }

    namespace internal
    {

        std::shared_ptr<ISurface> createSurface()
        {
            return std::static_pointer_cast<ISurface>(std::make_shared<WindowsSurface>());
        }

    }
}
