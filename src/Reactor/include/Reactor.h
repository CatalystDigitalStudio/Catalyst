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

    virtual void Run() override;


    virtual bool onEvent(TestEvent&& event) override;
};
