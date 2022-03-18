#include <IbWinCpp/Memory.hpp>

namespace ib {
    Addr::Addr(void* p) : p((Byte*)p) { }

    Addr::Addr(uintptr p) : p((Byte*)p) { }

    Addr::operator uintptr() const {
        return (uintptr)p;
    }

    Addr Addr::operator+(Offset offset) const {
        return p + offset;
    }
    Addr Addr::operator-(Offset offset) const {
        return p - offset;
    }

    Addr& Addr::operator+=(Offset offset) {
        p += offset;
        return *this;
    }
    Addr& Addr::operator-=(Offset offset) {
        p -= offset;
        return *this;
    }

    Addr Addr::operator[](Offset offset) const {
        return *(void**)(p + offset);
    }

    Addr::operator void* () const {
        return p;
    }

    Offset Addr::operator-(Addr addr) const {
        return p - addr.p;
    }

    [[nodiscard]] Addr Addr::align_up(size_t alignment) const {
        --alignment;
        return ((uintptr_t)p + alignment) & ~alignment;
    }
    [[nodiscard]] Addr Addr::align_down(size_t alignment) const {
        --alignment;
        return (uintptr_t)p & ~alignment;
    }

    Addr::read_return_auto Addr::read() const {
        return read_return_auto{ *this };
    }

    size_t Addr::heap_size(HANDLE heap) const {
        return HeapSize(heap, 0, p);
    }

    namespace mem {
        DWORD Protect::to_flProtect(ProtectFlags protect) {
            DWORD h = protect & ~0xFF;
            Byte l = (Byte)protect;
            ++l;
            if (l & Execute) {
                l = l << 4;
            }
            return h | l;
        }

        ProtectFlags Protect::to_protect(DWORD flProtect) {
            DWORD h = flProtect & ~0xFF;
            Byte l = (Byte)flProtect;
            --l;
            if (l & 0x8) {
                l = l >> 4 | Execute;
            }
            return h | l;
        }

        ProtectFlags change_protect(Addr addr, size size, ProtectFlags protect) {
            DWORD flProtect;
            VirtualProtect(addr, size, Protect::to_flProtect(protect), &flProtect);
            return Protect::to_protect(flProtect);
        }

        bool protect_changed(Addr addr, size size, ProtectFlags protect, std::function<void(Addr)> f) {
            DWORD flProtect;
            if (!VirtualProtect(addr, size, Protect::to_flProtect(protect), &flProtect))
                return false;
            f(addr);
            // lpflOldProtect can't be nullptr
            return VirtualProtect(addr, size, flProtect, &flProtect);
        }
    }
}