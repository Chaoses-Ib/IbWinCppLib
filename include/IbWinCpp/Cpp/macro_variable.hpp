#pragma once

namespace ib {
    namespace macro {
        /// _DEBUG
        constexpr bool _debug =
#ifdef _DEBUG
            true;
#else
            false;
#endif

        /// NDEBUG
        constexpr bool ndebug =
#ifdef NDEBUG
            true;
#else
            false;
#endif

        /// _WIN32
        constexpr bool _win32 =
#ifdef _WIN32
            true;
#else
            false;
#endif

        /// _WIN64
        constexpr bool _win64 =
#ifdef _WIN64
            true;
#else
            false;
#endif
    }

    constexpr bool debug_runtime = macro::_debug;
    constexpr bool debug_assert = macro::ndebug;

    /// @brief 32-bit Windows (x86 or 32-bit ARM)
    constexpr bool os_win32 = macro::_win32 && !macro::_win64;
    /// @brief 64-bit Windows (x64 or 64-bit ARM)
    constexpr bool os_win64 = macro::_win64;
}