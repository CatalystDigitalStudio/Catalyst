#pragma once

#ifndef CATALYST_HEADER_UTILITIES
#define CATALYST_HEADER_UTILITIES

namespace Catalyst
{

    template<typename T, T Value>
    struct Constant
    {
        static constexpr T value = Value;
        using type = T;
    };

    template<bool Value>
    using CatalystBool = Constant<bool, Value>;

    using catalyst_true = CatalystBool<true>;
    using catalyst_false = CatalystBool<false>;



    template<typename T, typename... Args>
    static constexpr T construct(Args&&... args)
    {
        static_assert(std::is_constructible_v<T, Args...>, "[CATALYST] [UTILITIES] T is not constructable!");
        return T(args...);
    }
    template<typename T, typename... Args>
    static constexpr T& construct_at(T* ptr, Args&&... args)
    {
        return *(new(ptr) T(args...));
    }


    template<typename Fn, typename... T>
    inline void for_each(Fn fn, T&&... args);
    template<typename Fn, typename T>
    inline void for_each(Fn fn, T&& arg)
    {
        fn(arg);
    }
    template<typename Fn, typename T, typename... R>
    inline void for_each(Fn fn, T&& arg, R&&... args)
    {
        fn(arg);
        for_each(args...);
    }

}

#endif //CATALYST_HEADER_UTILITIES