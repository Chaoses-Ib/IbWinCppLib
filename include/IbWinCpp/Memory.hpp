#pragma once
#include "Common.hpp"
#include <functional>

namespace ib {
    using Offset = intptr;  // size_t can't be negative

    class Addr {
    public:
        Byte* p;

        Addr(void* p);
        Addr(uintptr p);

        template<typename T>
        operator T*() const {
            return (T*)p;
        }

        /// @brief For method chaining. Same as operator T*().
        /// @internal Non-static data members can't have templates, so it has to be a function.
        template<typename T>
        T* ptr() const {
            return (T*)p;
        };

        explicit operator uintptr() const;


        Addr operator+(Offset offset) const;
        Addr operator-(Offset offset) const;

        /// @brief For method chaining. Same as operator+().
        [[nodiscard]] Addr offset(Offset offset) const {
            return p + offset;
        }

        /// @brief Equals to offset(sizeof(T) * num).
        template<typename T>
        [[nodiscard]] Addr offset(size_t num = 1) const {
            return p + sizeof(T) * num;
        }

        Addr& operator+=(Offset offset);
        Addr& operator-=(Offset offset);

        /// @brief Read the pointer at (p + offset).
        /// @details addr[off] equals to *(Addr*)(addr + off). ([addr + off] in assembly language)
        Addr operator[](Offset offset) const;


        /// @brief Functions as operator bool, operator<=>, operator delete
        operator void* () const;

        Offset operator-(Addr addr) const;


        [[nodiscard]] Addr align_up(size_t alignment) const;
        [[nodiscard]] Addr align_down(size_t alignment) const;


        template<typename T>
        T read() const {
            return *(T*)p;
        }


    private:
        struct read_return_auto {
            const Addr& addr;

            constexpr read_return_auto(const Addr& addr) : addr(addr) {}

            template<typename T>
            operator T() const {
                return addr.read<T>();
            }
        };
    public:
        read_return_auto read() const;

    public:

        template<typename T>
        void write(T value) const {
            *(T*)p = value;
        }

        // Return (size_t)-1 if fails.
        size_t heap_size(HANDLE heap = GetProcessHeap()) const;
    };

    namespace mem {
        /// @warning It does not equal to Windows's flProtect.
        using ProtectFlags = DWORD;
        class Protect {
        public:
            using T = const ProtectFlags;
            /// Converted to false (others are true).
            /// Cannot be used with other flags.
            /// Not supported by CreateFileMapping.
            static T NoAccess = PAGE_NOACCESS - 1;
            static T Read = PAGE_READONLY - 1;
            static T Write = PAGE_READWRITE - 1;  // Includes Read
            static T WriteCopy = PAGE_WRITECOPY - 1;  // Includes Read and Write. Not supported by VirtualAlloc(Ex).

            static T Execute = PAGE_EXECUTE;  // Not supported by CreateFileMapping if used without Read, Write or WriteCopy.

            static T Guard = PAGE_GUARD;  // Cannot be used with NoAccess, NoCache or WriteCombine
            static T NoCache = PAGE_NOCACHE;  // Cannot be used with NoAccess, Guard or WriteCombine
            static T WriteCombine = PAGE_WRITECOMBINE;  // Cannot be used with NoAccess, Guard or NoCache

            static T Targets_Invalid = PAGE_TARGETS_INVALID;  // Not supported by VirtualProtect or CreateFileMapping. Equals to Targets_NoUpdate.
            static T Targets_NoUpdate = PAGE_TARGETS_NO_UPDATE;  // Supported by VirtualProtect. Equals to Targets_Invalid.

            static DWORD to_flProtect(ProtectFlags protect);

            static ProtectFlags to_protect(DWORD flProtect);
        };

        /// Example: change_protect(addr, 5, Protect::Write | Protect::Execute);
        ProtectFlags change_protect(Addr addr, size size, ProtectFlags protect);

        /// @brief Change protect, call f, and then change protect back.
        bool protect_changed(Addr addr, size size, ProtectFlags protect, std::function<void(Addr)> f);
    }
}