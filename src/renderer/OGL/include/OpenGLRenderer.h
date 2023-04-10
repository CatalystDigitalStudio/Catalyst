#pragma once

#include "core/engine.h"
#include "core/logging.h"
#include "graphic/renderer.h"


class OpenGLRenderer : public Catalyst::IRenderer
{
    // Inherited via IRenderer
    virtual void initalize() override;
    virtual void cleanup() override;
    virtual void render() override;
    virtual const std::vector<Catalyst::IDevice*> getDeviceList() override;
    virtual Catalyst::PipelineID createPipeline(Catalyst::PipelineInformation) override;
    virtual std::shared_ptr<Catalyst::IPipeline> getPipeline(Catalyst::PipelineID) override;
    virtual Catalyst::CommandPool& getCommandPool() override;

public:
    OpenGLRenderer();
    virtual ~OpenGLRenderer();

private:
    Catalyst::CommandPool m_CommandPool;
};
