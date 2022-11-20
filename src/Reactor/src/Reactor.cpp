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
    CATALYST_LOG_ERROR(error.message);
}

static std::atomic<long> m_LastFrameTime;
static std::condition_variable m_Done;
static std::mutex m_Note;
void ReactorFrameLogger()
{
    while (!Catalyst::IApplication::get()->close())
    {
        std::unique_lock<std::mutex> lock(m_Note);
        m_Done.wait_until(lock, std::chrono::high_resolution_clock::now() + 20ms);
        CATALYST_LOG_INFO("Frame {0}ms", m_LastFrameTime);
    }
}


//struct TestScene : public Catalyst::Scene
//{
//
//    TestScene(Catalyst::IApplication* app)
//        : Scene("TestScene", app)
//    {
//
//    }
//
//    ~TestScene() = default;
//
//    virtual void onCreate() override
//    {
//        auto renderer = getApplication<Reactor>()->getRenderer();
//
//        
//        Catalyst::PipelineInformation info = {};
//
//        info.cullDirection = Catalyst::CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE;
//        info.cullFace = Catalyst::CATALYST_SHADER_CULL_FACE_BACK;
//        info.topology = Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP;
//
//        renderer->initalize();
//        renderer->createPipeline();
//        renderer->getPipeline()->initalize(info);
//    }
//    virtual void onUpdate() override
//    {
//
//        auto image = Catalyst::Engine::requestAsset<Catalyst::Image>("assets/image.png");
//
//        CATALYST_ASSET_EXISTS(image)
//        {
//
//        }
//
//        auto renderer = getApplication<Reactor>()->getRenderer();
//
//        //renderer->submit(model, material, ...);
//        //renderer->render();
//
//
//    }
//    virtual void onDestroy() override
//    {
//
//    }
//};

//void Reactor::startRenderer(Catalyst::CatalystPtrSurface surface)
//{
//
//    Catalyst::RendererInfo rInfo = {};
//
//    rInfo.type = Catalyst::CATALYST_RENDERER_TYPE_VULKAN;
//    rInfo.flags = (Catalyst::CATALYST_RENDERER_FLAG_DEVICE_DEFAULT | Catalyst::CATALYST_RENDERER_FLAG_DOUBLE_BUFFER);
//
//    m_Renderer.reset(Catalyst::Engine::createRenderer(surface, rInfo));
//
//}
//void Reactor::stopRenderer()
//{
//    m_Renderer->cleanup();
//}

Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{
    Catalyst::Engine::setErrorHandler(ReactorErrorHandler);
}
Reactor::~Reactor()
{
}
void Reactor::onRun()
{


    Catalyst::RendererInfo rInfo = {};

    rInfo.type = Catalyst::CATALYST_RENDERER_TYPE_VULKAN;
    rInfo.flags = (Catalyst::CATALYST_RENDERER_FLAG_DEVICE_DEFAULT | Catalyst::CATALYST_RENDERER_FLAG_DOUBLE_BUFFER);

    
    auto surface = Catalyst::Engine::createSurface({ "REACTOR" });

    m_Renderer.reset(Catalyst::Engine::createRenderer(surface, rInfo));

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                      PIPELINE                          */


    Catalyst::PipelineInformation info = {};

    info.cullDirection = Catalyst::CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE;
    info.cullFace = Catalyst::CATALYST_SHADER_CULL_FACE_BACK;
    info.topology = Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP;

    m_Renderer->initalize();

    Catalyst::PipelineID id = m_Renderer->createPipeline(info);
    Catalyst::PipelinePtr pipeline = m_Renderer->getPipeline(id);
    /*
    
    pipline->initalize(info);

    Catalyst::ShaderModule module = {};

    module...

    pipeline->createModule(module, ...);
    
    */

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                        MEMORY                          */

    //auto image = Catalyst::Engine::requestAsset<Catalyst::Image>("assets/image.png");
    //
    //renderer->bindTexture(image, 0);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    while (! close())
    {

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*                      COMMANDS                          */

        auto command = std::make_shared<Catalyst::BindPipelineCommand>(m_Renderer->getPipeline(id));

        m_Renderer->getCommandPool().reserve(10);
        m_Renderer->getCommandPool().add(std::static_pointer_cast<Catalyst::RenderCommandBase>(command));

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        auto sync_object = m_Renderer->render();


        //m_Renderer->wait(sync_object);

    }


    //CATALYST_PROFILE_FUNCTION(nullptr);
    //
    //auto surface = Catalyst::Engine::createSurface({ "REACTOR" });
    //
    //startRenderer(surface);
    //
    //std::thread logThread(ReactorFrameLogger);
    //Catalyst::Profiler profiler("Main loop");
    //
    ////launchScene<TestScene>();
    //
    //while (!close())
    //{
    //    profiler.start();
    //
    //    //getScene()->onUpdate();
    //
    //    getRenderer()->~IRenderer();
    //
    //    std::this_thread::sleep_for(10ms); //To save battery and resources on laptop rn. LOL!
    //
    //    Catalyst::Engine::updateEngine();
    //
    //    profiler.stop();
    //    m_LastFrameTime = profiler.count();
    //    m_Done.notify_all();
    //}
    //
    ////closeScene();
    //
    //stopRenderer();
    //
    //logThread.join();
}

bool Reactor::onEvent(TestEvent&& event)
{
    event.i;
    Catalyst::IApplication::get()->close(true);
    return true;
}
