#pragma once

/// @brief Force MSVC to evaluate a const expression at compile time.
#define IB_CONSTEXPR(kv) ib::as_constexpr<decltype(kv),kv>

namespace ib {
    /// @brief Force MSVC to evaluate a const expression at compile time.
    /// Alternative: IB_CONSTEXPR
    template<typename T, T A>
    constexpr T as_constexpr = A;
}