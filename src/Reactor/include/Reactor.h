#pragma once

#include "Catalyst.h"

class Reactor : public Catalyst::IApplication
{

public:
    Reactor();
    ~Reactor();

    virtual void Run() override;
};
