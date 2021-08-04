#pragma once
#include <Windows.h>
#include <cstdint>
#include <assert.h>

#include <string>
#include <iterator>
#include <functional>
#include <optional>

//For forcing MSVC to evaluate a const expression at compile time
#define CONSTEXPR(kv) ib::as_constexpr<decltype(kv),kv>

namespace ib {
#pragma region MacroVariables
    namespace macros {
        //_DEBUG
        const bool _debug =
#ifdef _DEBUG
            true;
#else
            false;
#endif

        //NDEBUG
        const bool ndebug =
#ifdef NDEBUG
            true;
#else
            false;
#endif

        //_WIN32
        const bool _win32 =
#ifdef _WIN32
            true;
#else
            false;
#endif

        //_WIN64
        const bool _win64 =
#ifdef _WIN64
            true;
#else
            false;
#endif
    }

    const bool debug_runtime = macros::_debug;
    const bool debug_assert = macros::ndebug;

    //32-bit Windows (x86 or 32-bit ARM)
    const bool os_win32 = macros::_win32 && !macros::_win64;
    //64-bit Windows (x64 or 64-bit ARM)
    const bool os_win64 = macros::_win64;

#pragma endregion

    using wchar = wchar_t;
    using Byte = uint8_t;  //since there is std::byte
    using QWORD = uint64_t;

    using std::function;
    using std::optional, std::nullopt;
    using std::wstring;



    template<typename T>
    class Holder;

    template<>
    class Holder<void> {
    public:
        struct DefaultT {} static inline Default;
    };

    template<typename T>
    class Holder {
        union {
            unsigned char buf[sizeof T];
            T v;
        };

    public:
#pragma warning(suppress : 26495)  //MEMBER_UNINIT
        Holder() {}
        ~Holder() {}

        Holder(Holder<void>::DefaultT) {
            new(buf) T();
        }

        template<typename... Ts>
        Holder(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
        }

        template<typename... Ts>
        void create(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
        }

        void destroy() {
            v.~T();
        }

        template<typename... Ts>
        void recreate(Ts&&... args) {
            destroy();
            new(buf) T(std::forward<Ts>(args)...);
        }

        T* operator->() {
            return &v;
        }

        const T* operator->() const {
            return &v;
        }

        T& operator*() {
            return v;
        }

        const T& operator*() const {
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
        HolderB() : b(false) {}
        ~HolderB() {}

        HolderB(Holder<void>::DefaultT) {
            new(buf) T();
            b = true;
        }

        template<typename... Ts>
        HolderB(Ts&&... args) {
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        bool has_created() const {
            return b;
        }

        template<typename... Ts>
        void create(Ts&&... args) {
            assert(!b);
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        // Do nothing if not created.
        void destroy() {
            if (b) {
                v.~T();
                b = false;
            }
        }

        // Destroy only if created.
        template<typename... Ts>
        void recreate(Ts&&... args) {
            destroy();
            new(buf) T(std::forward<Ts>(args)...);
            b = true;
        }

        // no assert(b) checks since accessing static members needed

        T* operator->() {
            return &v;
        }

        const T* operator->() const {
            return &v;
        }

        T& operator*() {
            return v;
        }

        const T& operator*() const {
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

        basic_zstring(TChar* p) : p(p) {}
        basic_zstring(TString s) {
            static_assert(std::is_const_v<TChar>, "ib::basic_zstring: Can't convert TString to non-const TChar*");
            p = s.c_str();
        }

        static basic_zstring New(size_t size) {
            return new TChar[size];
        }
        static basic_zstring New(TString s) {
            auto p = new TChar[s.size() + 1];
            memcpy(p, s.c_str(), (s.size() + 1) * sizeof TChar);
            return p;
        }

        //Set to nullptr
        void Delete() {
            delete[] p;
            p = nullptr;
        }

        operator TChar* () {
            return p;
        }
        operator TString () {
            return p;
        }

        operator bool() {
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
        auto_cast(T value) : value_(value) {};

        template<typename T2>
        operator T2() {
            return T2(value_);
        }
    };

#define IB_GENERATE_AUTO_XX_CAST(CAST)  \
    /*Enable C++17 to make use of auto type deduction.*/  \
    template<typename T>  \
    class auto_##CAST {  \
        T value_;  \
    public:  \
        auto_##CAST(T value) : value_(value) {};  \
        template<typename T2>  \
        operator T2() {  \
            return CAST<T2>(value_);  \
        }  \
    };

    IB_GENERATE_AUTO_XX_CAST(static_cast);
    IB_GENERATE_AUTO_XX_CAST(const_cast);
    IB_GENERATE_AUTO_XX_CAST(dynamic_cast);
    IB_GENERATE_AUTO_XX_CAST(reinterpret_cast);
#undef IB_GENERATE_AUTO_XX_CAST

}