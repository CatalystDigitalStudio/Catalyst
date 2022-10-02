#include "DX11Renderer.h"
#include "Catalyst.h"

#include "../platform/windows/windowsSurface.h"

extern "C" CATALYST_API Catalyst::IRenderer * createRenderer()
{
    return new DX11Renderer();
}

void DX11Renderer::initalizeSwapchain()
{
}

void DX11Renderer::initalizeDevices()
{
}

void DX11Renderer::initalize(std::shared_ptr<Catalyst::ISurface> surface)
{
    Catalyst::Logger::initalizeDefault();

    m_Surface = surface;

    DXGI_SWAP_CHAIN_DESC sd = {};
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 640;
    sd.BufferDesc.Height = 480;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = std::static_pointer_cast<Catalyst::WindowsSurface>(surface)->getWindowHandle();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
}
void DX11Renderer::cleanup()
{

}

void DX11Renderer::addPipline()
{

}


DX11Device::DX11Device()
{

}
DX11Device::~DX11Device()
{

}

const unsigned int DX11Device::getID()
{
    return 0;
}
const bool DX11Device::getProperty(Property)
{
    return false;
}
void DX11Device::setProperty(Property, bool value)
{

}
