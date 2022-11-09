#include "pch.h"
#include "internal.h"

namespace Catalyst
{


}

#if CATALYST_TRACK_ALLOCATIONS

CATALYST_LOGIC_DISCARD void* operator new(std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    if (!ptr)
        throw;

    Catalyst::Engine::addAllocation(size);

    return ptr;
}
CATALYST_LOGIC_DISCARD void* operator new[](std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    Catalyst::Engine::addAllocation(size);

    if (!ptr)
        throw;

    Catalyst::CatalystGetAllocations();

    return ptr;
}
void operator delete(void* ptr) noexcept
{
    Catalyst::Engine::removeAllocation(_msize(ptr));
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    Catalyst::Engine::removeAllocation(_msize(ptr));
    free(ptr);
}
#endif