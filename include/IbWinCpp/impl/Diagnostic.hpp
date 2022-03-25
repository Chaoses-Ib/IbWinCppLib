#include "../Common.hpp"
#include <type_traits>

namespace ib {
    template<typename CharT, typename TraitsT>
    inline DebugStreamBuf<CharT, TraitsT>::DebugStreamBuf() : line_prefix() {}

    template<typename CharT, typename TraitsT>
    inline DebugStreamBuf<CharT, TraitsT>::DebugStreamBuf(StringT&& line_prefix) : line_prefix(std::move(line_prefix)) {}

    template<typename CharT, typename TraitsT>
    inline DebugStreamBuf<CharT, TraitsT>::DebugStreamBuf(const CharT* line_prefix) : DebugStreamBuf(StringT(line_prefix)) {}

    template<typename CharT, typename TraitsT>
    inline DebugStreamBuf<CharT, TraitsT>::DebugStreamBuf(StringViewT line_prefix) : DebugStreamBuf(StringT(line_prefix)) {}

    template<typename CharT, typename TraitsT>
    inline DebugStreamBuf<CharT, TraitsT>::~DebugStreamBuf() {
        sync();
    }

    template<typename CharT, typename TraitsT>
    inline int DebugStreamBuf<CharT, TraitsT>::sync() {
        StringT buf = this->str();
        if (buf.empty())  // needed
            return 0;
        if (!line_prefix.empty()) {
            buf = line_prefix + buf;
        }
        const CharT* str = buf.c_str();

        if constexpr (std::is_same_v<CharT, wchar>) {
            OutputDebugStringW(str);
        }
        else if constexpr (std::is_same_v<CharT, char>) {
            OutputDebugStringA(str);
        }
        else {
            static_assert(/* false */ !sizeof(CharT*), "ib::DebugStreamBuf: unsupported CharT");
        }
        this->str(std::basic_string<CharT>());
        return 0;
    }

    template<typename CharT, typename TraitsT>
    inline DebugOStream<CharT, TraitsT>::DebugOStream() : std::basic_ostream<CharT, TraitsT>(new DebugStreamBuf<CharT, TraitsT>()) {}

    template<typename CharT, typename TraitsT>
    inline DebugOStream<CharT, TraitsT>::DebugOStream(StringT&& line_prefix) : std::basic_ostream<CharT, TraitsT>(new DebugStreamBuf<CharT, TraitsT>(std::move(line_prefix))) {}

    template<typename CharT, typename TraitsT>
    inline DebugOStream<CharT, TraitsT>::DebugOStream(const CharT* line_prefix) : DebugOStream(StringT(line_prefix)) {}

    template<typename CharT, typename TraitsT>
    inline DebugOStream<CharT, TraitsT>::DebugOStream(StringViewT line_prefix) : DebugOStream(StringT(line_prefix)) {}

    template<typename CharT, typename TraitsT>
    inline DebugOStream<CharT, TraitsT>::~DebugOStream() {
        delete this->rdbuf();
    }
}