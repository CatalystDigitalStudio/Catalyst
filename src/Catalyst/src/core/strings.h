#pragma once

#include <malloc.h>
#include <xtr1common>

#include "internal.h"

#ifndef CATALYEST_STRINGS_HEADER
#define CATALYEST_STRINGS_HEADER

namespace Catalyst
{


    template<typename Character, size_t Limit>
    struct basic_sso
    {
        Character SSO[Limit];
        Character* Large;

        CATALYST_LOGIC_DISCARD inline constexpr Character* allocate(const size_t amount)
        {
            const size_t size = amount * sizeof(Character);

            if (size <= Limit)
                return &SSO[0];
            else
                return (Character*)_aligned_malloc(size, alignof(Character));
        }
        CATALYST_LOGIC_DISCARD inline constexpr Character* reallocate(const Character* location, const size_t original, const size_t ammount)
        {
            const size_t final_size = original + amount;
            if (final_size <= Limit)
            {
                free(location);
                return &SSO[0];
            }
            else if (!_aligned_msize(location, alignof(Character), amount))
            {
                free(location);
                return allocate(final_size);
            }
            else
            {
                return location;
            }
        }
        inline constexpr void free(Character* ptr)
        {
            if(ptr && ptr != &SSO[0])
                _aligned_free(ptr);
        }
    };

    template<typename Character>
    class basic_string
    {

        static_assert(std::_Is_any_of_v<Character, char, char8_t, char16_t, char32_t, wchar_t>, "Character type must a basic type");

    public:
        using type = character;
        using pointer = character*;
        using reference = character&;

    public:
        inline constexpr basic_string();
        inline constexpr ~basic_string();

    private:
        inline constexpr void construct(char* str)
        {
        }

    private:
        basic_sso<Character, 32> m_Allocator;
        Character* m_Begin;
        Character* m_End;
        size_t m_Capacity;
    };

}

#endif //CATALYEST_STRINGS_HEADER