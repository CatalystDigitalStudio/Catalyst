#pragma once

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

    using catalyst_true  = CatalystBool<true>;
    using catalyst_false = CatalystBool<false>;

    template<typename Base, typename Derived>
    struct CatalystBaseOf : CatalystBool<__is_base_of(Base, Derived)> { };

    template<typename A, typename B>
    struct CatalystCompare : catalyst_false { };
    template<typename A>
    struct CatalystCompare<A, A> : catalyst_true { };

    template<bool Enable, typename T = void>
    struct CatalystEnableIf
    {
        static_assert(Enable, "Unable to satisfy enable statement");
    };
    template<typename T>
    struct CatalystEnableIf<true, T>
    {
        using type = T;
    };
}
