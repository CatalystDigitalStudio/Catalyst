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


struct TestScene : public Catalyst::Scene
{

    TestScene(Catalyst::IApplication* app)
        : Scene("TestScene", app)
    {

    }

    ~TestScene() = default;

    virtual void onCreate() override
    {
        auto renderer = getApplication<Reactor>()->getRenderer();

    }
    virtual void onUpdate() override
    {

        auto image = Catalyst::Engine::requestAsset<Catalyst::Image>("assets/image.png");

        CATALYST_ASSET_EXISTS(image)
        {

        }

    }
    virtual void onDestroy() override
    {

    }
};

void Reactor::startRenderer(Catalyst::CatalystPtrSurface surface)
{

    Catalyst::RendererInfo rInfo = {};

    rInfo.type = Catalyst::CATALYST_RENDERER_TYPE_VULKAN;
    rInfo.flags = (Catalyst::CATALYST_RENDERER_FLAG_DEVICE_DEFAULT | Catalyst::CATALYST_RENDERER_FLAG_DOUBLE_BUFFER);

    m_Renderer.reset(Catalyst::Engine::createRenderer(surface, rInfo));

    Catalyst::PipelineInformation info = {};

    info.cullDirection = Catalyst::CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE;
    info.cullFace = Catalyst::CATALYST_SHADER_CULL_FACE_BACK;
    info.topology = Catalyst::CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP;

    m_Renderer->initalize();
    m_Renderer->createPipeline();
    m_Renderer->getPipeline()->initalize(info);
}
void Reactor::stopRenderer()
{
    m_Renderer->cleanup();
}

Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{
    Catalyst::Engine::setErrorHandler(ReactorErrorHandler);
}
Reactor::~Reactor()
{
}
void Reactor::Run()
{
    CATALYST_PROFILE_FUNCTION(nullptr);

    auto surface = Catalyst::Engine::createSurface({ "REACTOR" });

    startRenderer(surface);

    std::thread logThread(ReactorFrameLogger);
    Catalyst::Profiler profiler("Main loop");

    launchScene<TestScene>();

    while (!close())
    {
        profiler.start();

        getScene()->onUpdate();

        std::this_thread::sleep_for(10ms); //To save battery and resources on laptop rn. LOL!

        Catalyst::Engine::updateEngine();

        profiler.stop();
        m_LastFrameTime = profiler.count();
        m_Done.notify_all();
    }

    closeScene();

    stopRenderer();

    logThread.join();
}

bool Reactor::onEvent(TestEvent&& event)
{
    event.i;
    Catalyst::IApplication::get()->close(true);
    return true;
}
