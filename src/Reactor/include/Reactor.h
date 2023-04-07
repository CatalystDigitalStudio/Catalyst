#pragma once

#include "Catalyst.h"

struct TestEvent : public Catalyst::IEvent<TestEvent>
{
    unsigned int i = 0;

    TestEvent(unsigned int i) : i(i) { ; }
};

class Reactor : public Catalyst::IApplication, Catalyst::IListener<TestEvent>
{

public:
    Reactor();
    ~Reactor();

    inline std::shared_ptr<Catalyst::IRenderer> getRenderer() const { return m_Renderer; }

private:
    void startRenderer(Catalyst::CatalystPtrSurface);
    void stopRenderer();

private:
    virtual void onRun() override;

    virtual bool onEvent(TestEvent&& event) override;

private:
    std::shared_ptr<Catalyst::IRenderer> m_Renderer;
};
