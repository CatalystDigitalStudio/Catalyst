#define CATALYST_MAIN

#define CATALYST_ENABLE_PROFILING
#define CATALYST_ENABLE_CORE_PROFILING

#include "Reactor.h"

#include <future>
#include <fstream>
#include <streambuf>

#include <iostream>

using namespace std::chrono_literals;

CATALYST_LAUNCH(Reactor);

void ReactorErrorHandler(Catalyst::CatalystError&& error)
{
    //CATALYST_LOG_ERROR("CATALYST ERROR HANDLER : {0}", error.message);
}


Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{
    Catalyst::setErrorHandler(ReactorErrorHandler);
}
Reactor::~Reactor()
{

}
void Reactor::onRun()
{


    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                      RENDERER                          */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    Catalyst::RendererInfo rInfo = {};

    rInfo.type = Catalyst::CATALYST_RENDERER_TYPE_OPENGL;
    rInfo.flags = Catalyst::CATALYST_RENDERER_FLAG_DEVICE_DEFAULT;
    

    m_Renderer = Catalyst::createRenderer(rInfo);

    Catalyst::Surface surface = {};

    surface.create("", 0, 0);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                      PIPELINE                          */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


    Catalyst::PipelineInformation info = {};

    info.cullDirection = Catalyst::CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE;
    info.cullFace = Catalyst::CATALYST_SHADER_CULL_FACE_BACK;
    info.topology = Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP;

    m_Renderer->initalize();

    Catalyst::PipelineID id = m_Renderer->createPipeline(info);
    Catalyst::PipelinePtr pipeline = m_Renderer->getPipeline(id);
    
}

bool Reactor::onEvent(TestEvent&& event)
{
    event.i;
    Catalyst::IApplication::get()->close(true);
    return true;
}
