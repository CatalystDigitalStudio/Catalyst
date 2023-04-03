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

static std::atomic<long long> m_FPS;
static std::atomic<std::chrono::nanoseconds> m_LastFrameTime;
static std::condition_variable m_Done;
static std::mutex m_Note;
void ReactorFrameLogger()
{
    while (!Catalyst::IApplication::get()->close())
    {
#if 0
        std::unique_lock<std::mutex> lock(m_Note);
        m_Done.wait_until(lock, std::chrono::high_resolution_clock::now() + 2ms);
        CATALYST_LOG_INFO("Frame {0}\xE6s", std::chrono::duration_cast<std::chrono::microseconds>(m_LastFrameTime.load()).count());
#else
        std::this_thread::sleep_for(1s);
        CATALYST_LOG_INFO("Frames: {0}", m_FPS);
        m_FPS = 0;
#endif
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

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                      RENDERER                          */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    Catalyst::RendererInfo rInfo = {};

    rInfo.type = Catalyst::CATALYST_RENDERER_TYPE_OPENGL;
    rInfo.flags = (Catalyst::CATALYST_RENDERER_FLAG_DEVICE_DEFAULT);
    
    auto surface = Catalyst::Engine::createSurface({ "REACTOR" });

    m_Renderer.reset(Catalyst::Engine::createRenderer(surface, rInfo));


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
    /*
    
    pipline->initalize(info);

    Catalyst::ShaderModule module = {};

    module...

    pipeline->createModule(module, ...);
    
    */

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                        MEMORY                          */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    //auto image = Catalyst::Engine::requestAsset<Catalyst::Image>("assets/image.png");
    //
    //renderer->bindTexture(image, 0);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    auto command = std::make_shared<Catalyst::BindPipelineCommand>(id);

    m_Renderer->getCommandPool().reserve(10);

    std::thread logThread(ReactorFrameLogger);
    Catalyst::Profiler profiler("Main loop");

    while (! close())
    {
        //profiler.start();

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*                      COMMANDS                          */
        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        //m_Renderer->getCommandPool().add(std::static_pointer_cast<Catalyst::RenderCommandBase>(command));
        
        //m_Renderer->getCommandPool().flush();

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        //auto sync_object = m_Renderer->render();


        //m_Renderer->wait(sync_object);

        surface->update();

        //profiler.stop();
        //m_LastFrameTime = profiler.count();
        //m_Done.notify_all();

        //std::this_thread::sleep_for(10ms); //To save resources

        ++m_FPS;
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

    logThread.join();
}

bool Reactor::onEvent(TestEvent&& event)
{
    event.i;
    Catalyst::IApplication::get()->close(true);
    return true;
}
