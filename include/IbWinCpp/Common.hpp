#pragma once
#include <Windows.h>
#include <cstdint>
#include <assert.h>

#include <string>
#include <iterator>
#include <functional>

//For forcing MSVC to evaluate a const expression at compile time
#define CONSTEXPR(kv) ib::as_constexpr<decltype(kv),kv>

namespace ib {
#pragma region MacroVariables
    namespace macros {
        //_DEBUG
        constexpr bool _debug =
#ifdef _DEBUG
            true;
#else
            false;
#endif

        //NDEBUG
        constexpr bool ndebug =
#ifdef NDEBUG
            true;
#else
            false;
#endif

        //_WIN32
        constexpr bool _win32 =
#ifdef _WIN32
            true;
#else
            false;
#endif

        //_WIN64
        constexpr bool _win64 =
#ifdef _WIN64
            true;
#else
            false;
#endif
    }

    constexpr bool debug_runtime = macros::_debug;
    constexpr bool debug_assert = macros::ndebug;

    //32-bit Windows (x86 or 32-bit ARM)
    constexpr bool os_win32 = macros::_win32 && !macros::_win64;
    //64-bit Windows (x64 or 64-bit ARM)
    constexpr bool os_win64 = macros::_win64;

#pragma endregion

    using wchar = wchar_t;
    using Byte = uint8_t;  //since there is std::byte
    using QWORD = uint64_t;

    using std::function;
    using std::wstring;



    template<typename T>
    class Holder;

    template<>
    class Holder<void> {
    public:
        struct DefaultT {} static constexpr Default{};
    };

    template<typename T>
    class Holder {
        union {
            unsigned char buf[sizeof T];
            T v;
        };

    public:
#pragma warning(suppress : 26495)  //MEMBER_UNINIT
        constexpr Holder() {}
        ~Holder() {}

        constexpr Holder(Holder<void>::DefaultT) {
            new(buf) T();
        }

        template<typename... Ts>
        constexpr Holder(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        constexpr void create(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
        }

        constexpr void destroy() {
            v.~T();
        }

        template<typename... Ts>
        constexpr void recreate(Ts&&... args) {
            destroy();
            new(buf) T(std::forward<Ts>(args)...);
        }

        constexpr T* operator->() {
            return &v;
        }

        constexpr const T* operator->() const {
            return &v;
        }

        constexpr T& operator*() {
            return v;
        }

        constexpr const T& operator*() const {
            return v;
        }
    };

    // Thread-unsafe.
    template<typename T>
    class HolderB{
        union {
            unsigned char buf[sizeof T];
            T v;
        };
        bool b;

    public:
#pragma warning(suppress : 26495)  //MEMBER_UNINIT
        constexpr HolderB() : b(false) {}
        ~HolderB() {}

        constexpr HolderB(Holder<void>::DefaultT) {
            new(buf) T();
            b = true;
        }

        template<typename... Ts>
        constexpr HolderB(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        constexpr bool has_created() const {
            return b;
        }

        template<typename... Ts>
        constexpr void create(Ts&&... args) {
            assert(!b);
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        // Do nothing if not created.
        constexpr void destroy() {
            if (b) {
                v.~T();
                b = false;
            }
        }

        // Destroy only if created.
        template<typename... Ts>
        constexpr void recreate(Ts&&... args) {
            destroy();
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        // no assert(b) checks since accessing static members needed

        constexpr T* operator->() {
            return &v;
        }

        constexpr const T* operator->() const {
            return &v;
        }

        constexpr T& operator*() {
            return v;
        }

        constexpr const T& operator*() const {
            return v;
        }
    };



    //Alternative std::iterator
    template<typename TCategory, typename TValue, typename TDiff = ptrdiff_t, typename TPointer = TValue*, typename TReference = TValue&>
    class iterator {
    public:
        using iterator_category = TCategory;
        using value_type = TValue;
        using difference_type = TDiff;
        using pointer = TPointer;
        using reference = TReference;
    };

    //For forcing MSVC to evaluate a const expression at compile time
    //Alternative: CONSTEXPR
    template<typename T, T A>
    constexpr T as_constexpr = A;

    //A simple wrapper for TChar*
    //Could be nullptr
    template<typename TChar, typename TString>
    class basic_zstring {
    public:
        TChar* p;

        constexpr basic_zstring(TChar* p) : p(p) {}
        constexpr basic_zstring(TString s) {
            static_assert(std::is_const_v<TChar>, "ib::basic_zstring: Can't convert TString to non-const TChar*");
            p = s.c_str();
        }

        constexpr static basic_zstring New(size_t size) {
            return new TChar[size];
        }
        constexpr static basic_zstring New(TString s) {
            auto p = new TChar[s.size() + 1];
            //memcpy(p, s.c_str(), (s.size() + 1) * sizeof TChar);
            std::copy(s.begin(), s.end(), p);
            p[s.size()] = static_cast<TChar>(0);
            return p;
        }

        //Set to nullptr
        constexpr void Delete() {
            delete[] p;
            p = nullptr;
        }

        constexpr operator TChar* () {
            return p;
        }
        constexpr operator TString () {
            return p;
        }

        constexpr operator bool() {
            return p;
        }
    };
    
    using cwzstring = basic_zstring<const wchar, const wstring>;
    using wzstring = basic_zstring<wchar, wstring>;

    //Enable C++17 to make use of auto type deduction.
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
    /*Enable C++17 to make use of auto type deduction.*/  \
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