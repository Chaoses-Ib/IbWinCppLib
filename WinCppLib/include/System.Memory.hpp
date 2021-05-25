#pragma once
#include "Common.hpp"

namespace ib {
    using Offset = intptr_t;  // size_t can't be negative

    class Addr {
    public:
        void* p;

        Addr(void* p) : p(p) { }
        Addr(uintptr_t p) : p((void*)p) { }

        template<typename T>
        operator T* () {
            return (T*)p;
        }

        // For method chaining. Same as operator T*().
        // Non-static data members can't have templates, so it has to be a function.
        template<typename T>
        T* ptr() {
            return (T*)p;
        };

        explicit operator uintptr_t() {
            return (uintptr_t)p;
        }



        Addr operator+(Offset offset) const {
            return (Byte*)p + offset;
        }
        Addr operator-(Offset offset) const {
            return (Byte*)p - offset;
        }

        // For method chaining. Same as operator+().
        [[nodiscard]] Addr offset(Offset offset) {
            return *this + offset;
        }

        //Equals to offset(sizeof(T) * num).
        template<typename T>
        [[nodiscard]] Addr offset(size_t num = 1) {
            return offset(sizeof(T) * num);
        }

        Addr& operator+=(Offset offset) {
            p = (Byte*)p + offset;
            return *this;
        }
        Addr& operator-=(Offset offset) {
            p = (Byte*)p - offset;
            return *this;
        }

        // Read the pointer at (p + offset).
        // addr[off] equals to *(Addr*)(addr + off). ([addr + off] in assembly language)
        Addr operator[](Offset offset) const {
            return *(void**)(*this + offset);
        }
        


        // Check nullptr.
        operator bool() {
            return p;
        }

        Offset operator-(Addr addr) {
            return (intptr_t)p - (intptr_t)addr.p;
        }

        auto operator<=>(const Addr addr) const {
            return p <=> addr.p;
        }



        [[nodiscard]] Addr align_up(size_t alignment) {
            --alignment;
            return ((uintptr_t)p + alignment) & ~alignment;
        }
        [[nodiscard]] Addr align_down(size_t alignment) {
            --alignment;
            return (uintptr_t)p & ~alignment;
        }



        template<typename T>
        T read() {
            return *(T*)p;
        }

        auto read() {
            return read_return_auto{ *this };
        }
    private:
        struct read_return_auto {
            Addr& addr;

            template<typename T>
            operator T() {
                return addr.read<T>();
            }
        };
    public:

        template<typename T>
        void write(T value) {
            *(T*)p = value;
        }

        

        // Return (size_t)-1 if fails.
        size_t heap_size(HANDLE heap = GetProcessHeap()) {
            return HeapSize(heap, 0, p);
        }
    };
}