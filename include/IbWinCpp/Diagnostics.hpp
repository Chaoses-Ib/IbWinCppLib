#pragma once
#include "Common.hpp"
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace ib {
    /*
    * References
    * Sven, Axelsson. Using an output stream for debugging. https://gist.github.com/comargo/f7fe244e539325cef0f66704b58dc35a
    */

    template <typename CharT = wchar, typename TraitsT = std::char_traits<CharT>>
    class DebugStreamBuf : public std::basic_stringbuf<CharT, TraitsT>
    {
    public:
        using StringT = std::basic_string<CharT, TraitsT>;
        using StringViewT = std::basic_string_view<CharT, TraitsT>;

        DebugStreamBuf() : line_prefix() {}
        DebugStreamBuf(StringT&& line_prefix) : line_prefix(std::move(line_prefix)) {}
        DebugStreamBuf(const CharT* line_prefix) : DebugStreamBuf(StringT(line_prefix)) {}
        DebugStreamBuf(StringViewT line_prefix) : DebugStreamBuf(StringT(line_prefix)) {}

        virtual ~DebugStreamBuf() {
            sync();
        }
    protected:
        int sync() {
            StringT buf = this->str();
            if (buf.empty())  //needed
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
                static_assert(false, "DebugStreamBuf: unsupported CharT");
            }
            this->str(std::basic_string<CharT>());
            return 0;
        }
    private:
        const StringT line_prefix;
    };

    template <typename CharT = wchar, typename TraitsT = std::char_traits<CharT>>
    class DebugOStream : public std::basic_ostream<CharT, TraitsT>
    {
    public:
        using StringT = std::basic_string<CharT, TraitsT>;
        using StringViewT = std::basic_string_view<CharT, TraitsT>;

        DebugOStream() : std::basic_ostream<CharT, TraitsT>(new DebugStreamBuf<CharT, TraitsT>()) {}
        DebugOStream(StringT&& line_prefix) : std::basic_ostream<CharT, TraitsT>(new DebugStreamBuf<CharT, TraitsT>(std::move(line_prefix))) {}
        DebugOStream(const CharT* line_prefix) : DebugOStream(StringT(line_prefix)) {}
        DebugOStream(StringViewT line_prefix) : DebugOStream(StringT(line_prefix)) {}

        ~DebugOStream() {
            delete this->rdbuf();
        }
    };
}