#pragma once

#include "graphic/renderer.h"

#include <d3d11.h>
//#include <d3dx11.h>
//#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx11.lib")
//#pragma comment (lib, "d3dx10.lib")



class CATALYST_API DX11Device : public Catalyst::IDevice
{

public:
    DX11Device();
    ~DX11Device();

private:
    virtual const unsigned int getID() override;
    virtual const bool getProperty(Property) override;
    virtual void setProperty(Property, bool value) override;

private:
};

class CATALYST_API DX11Renderer : public Catalyst::IRenderer
{

public:
    DX11Renderer() = default;
    ~DX11Renderer() = default;

    virtual void initalizeSwapchain() override;
    virtual void initalizeDevices() override;

private:
    virtual void initalize(std::shared_ptr<Catalyst::ISurface>) override;
    virtual void cleanup() override;

    virtual void addPipline() override;

private:
    DXGI_SWAP_CHAIN_DESC swapChain = {};
};
