#pragma once
#include "Common.hpp"

namespace ib {
    using Offset = intptr_t;  // size_t can't be negative

    class Addr {
    public:
        Byte* p;

        Addr(void* p) : p((Byte*)p) { }
        Addr(uintptr_t p) : p((Byte*)p) { }

        template<typename T>
        operator T*() const {
            return (T*)p;
        }

        // For method chaining. Same as operator T*().
        // Non-static data members can't have templates, so it has to be a function.
        template<typename T>
        T* ptr() const {
            return (T*)p;
        };

        explicit operator uintptr_t() const {
            return (uintptr_t)p;
        }



        Addr operator+(Offset offset) const {
            return p + offset;
        }
        Addr operator-(Offset offset) const {
            return p - offset;
        }

        // For method chaining. Same as operator+().
        [[nodiscard]] Addr offset(Offset offset) const {
            return p + offset;
        }

        //Equals to offset(sizeof(T) * num).
        template<typename T>
        [[nodiscard]] Addr offset(size_t num = 1) const {
            return p + sizeof(T) * num;
        }

        constexpr Addr& operator+=(Offset offset) {
            p += offset;
            return *this;
        }
        constexpr Addr& operator-=(Offset offset) {
            p -= offset;
            return *this;
        }

        // Read the pointer at (p + offset).
        // addr[off] equals to *(Addr*)(addr + off). ([addr + off] in assembly language)
        Addr operator[](Offset offset) const {
            return *(void**)(p + offset);
        }
        


        // Function as operator bool, operator<=>, operator delete
        constexpr operator void* () const {
            return p;
        }

        Offset operator-(Addr addr) const {
            return p - addr.p;
        }



        [[nodiscard]] Addr align_up(size_t alignment) const {
            --alignment;
            return ((uintptr_t)p + alignment) & ~alignment;
        }
        [[nodiscard]] Addr align_down(size_t alignment) const {
            --alignment;
            return (uintptr_t)p & ~alignment;
        }



        template<typename T>
        T read() const {
            return *(T*)p;
        }

        constexpr auto read() const {
            return read_return_auto{ *this };
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

        template<typename T>
        void write(T value) const {
            *(T*)p = value;
        }

        

        // Return (size_t)-1 if fails.
        size_t heap_size(HANDLE heap = GetProcessHeap()) const {
            return HeapSize(heap, 0, p);
        }
    };

    namespace mem {
        // **It does not equal to Windows's flProtect.**
        using ProtectFlags = DWORD;
        class Protect {
        public:
            using T = const ProtectFlags;
            /// <summary>
            /// Converted to false (others are true).
            /// Cannot be used with other flags.
            /// Not supported by CreateFileMapping.
            /// </summary>
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

            constexpr static DWORD to_flProtect(ProtectFlags protect) {
                DWORD h = protect & ~0xFF;
                Byte l = (Byte)protect;
                ++l;
                if (l & Execute) {
                    l = l << 4;
                }
                return h | l;
            }

            constexpr static ProtectFlags to_protect(DWORD flProtect) {
                DWORD h = flProtect & ~0xFF;
                Byte l = (Byte)flProtect;
                --l;
                if (l & 0x8) {
                    l = l >> 4 | Execute;
                }
                return h | l;
            }
        };

        // Example: change_protect(addr, 5, Protect::Write | Protect::Execute);
        inline ProtectFlags change_protect(Addr addr, size_t size, ProtectFlags protect) {
            DWORD flProtect;
            VirtualProtect(addr, size, Protect::to_flProtect(protect), &flProtect);
            return Protect::to_protect(flProtect);
        }

        // Change protect, call f, and then change protect back.
        inline bool protect_changed(Addr addr, size_t size, ProtectFlags protect, function<void(Addr)> f) {
            DWORD flProtect;
            if (!VirtualProtect(addr, size, Protect::to_flProtect(protect), &flProtect))
                return false;
            f(addr);
            //lpflOldProtect can't be nullptr
            return VirtualProtect(addr, size, flProtect, &flProtect);
        }
    }
}