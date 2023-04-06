#pragma once

namespace Catalyst
{

    template<typename T>
    static constexpr void copy(const T* dest_begin, const T* dest_end, const T* src_begin, const T* src_end)
    {
        CATALYST_DEBUG_ASSERT(dest_begin && dest_end && src_begin && src_end, Engine::raiseError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));

        const size_t s_size = src_end - src_begin;
        const size_t d_size = dest_end - dest_begin;

        CATALYST_DEBUG_ASSERT(dest_begin < dest_end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin, __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(src_begin < src_end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(d_size >= s_size, Engine::raiseError({ Level::Error, "CatalystResult::Destination_Is_Smaller_Than_Source", CatalystResult::Destination_Is_Smaller_Than_Source, __FUNCTION__ }));

        T* d_begin = (T*)dest_begin;

        while (src_begin != src_end)
        {
            *d_begin = *src_begin;
            ++d_begin;
            ++src_begin;
        }
    }

    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, T&& delimiter)
    {
        CATALYST_DEBUG_ASSERT(begin && end, Engine::raiseError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(begin < end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));

        while (begin != end)
        {
            if (*begin == delimiter)
                return begin;
            else
                ++begin;
        }

        return nullptr;
    }

    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, const T* delimiter_begin, const T* delimiter_end)
    {
        CATALYST_DEBUG_ASSERT(begin && end && delimiter_begin && delimiter_end, Engine::raiseError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));

        CATALYST_DEBUG_ASSERT(begin < end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(delimiter_begin < delimiter_end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));

        while (begin != end)
        {
            if (*begin == *delimiter_begin)
            {
                const T* b_index = begin;
                bool found = true;
                for (const T* i = delimiter_begin; i != (delimiter_end - 1); i++, begin++)
                {
                    if (*i != *begin)
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                {
                    return b_index;
                }
            }
            else
            {
                ++begin;
            }
        }

        return nullptr;
    }
    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* count_KMP(const T* begin, const T* end, const T* delimiter_begin, const T* delimiter_end)
    {
        CATALYST_DEBUG_ASSERT(begin && end && delimiter_begin && delimiter_end, Engine::raiseError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));

        CATALYST_DEBUG_ASSERT(begin < end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(delimiter_begin < delimiter_end, Engine::raiseError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));

    }

}
