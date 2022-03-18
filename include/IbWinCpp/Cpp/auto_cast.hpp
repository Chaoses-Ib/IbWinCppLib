#pragma once

namespace ib {
    /// @note C++17 is needed to make use of auto type deduction.
    template<typename T>
    class auto_cast {
        T value_;
    public:
        constexpr auto_cast(T value) : value_(value) {};

        template<typename T2>
        constexpr operator T2() {
            return T2(value_);
        }
    };

#define IB_GENERATE_AUTO_XX_CAST(CAST)  \
    /** @note C++17 is needed to make use of auto type deduction. */  \
    template<typename T>  \
    class auto_##CAST {  \
        T value_;  \
    public:  \
        constexpr auto_##CAST(T value) : value_(value) {};  \
        template<typename T2>  \
        constexpr operator T2() {  \
            return CAST<T2>(value_);  \
        }  \
    };

    IB_GENERATE_AUTO_XX_CAST(static_cast);
    IB_GENERATE_AUTO_XX_CAST(const_cast);
    IB_GENERATE_AUTO_XX_CAST(dynamic_cast);
    IB_GENERATE_AUTO_XX_CAST(reinterpret_cast);
#undef IB_GENERATE_AUTO_XX_CAST
}