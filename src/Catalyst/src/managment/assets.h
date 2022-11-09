#pragma once

#include "manager.h"

namespace Catalyst
{

    template<typename T>
    using AssetManager = IManagerShared<std::string, T>;


}
