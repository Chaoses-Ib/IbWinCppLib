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
        virtual ~DebugStreamBuf() {
            sync();
        }
    protected:
        int sync() {
            if constexpr (std::is_same_v<CharT, wchar>) {
                OutputDebugStringW(this->str().c_str());
            }
            else if constexpr (std::is_same_v<CharT, char>) {
                OutputDebugStringA(this->str().c_str());
            }
            else {
                static_assert(false, "DebugStreamBuf: unsupported CharT");
            }
            this->str(std::basic_string<CharT>());
            return 0;
        }
    };

    template <typename CharT = wchar, typename TraitsT = std::char_traits<CharT>>
    class DebugOStream : public std::basic_ostream<CharT, TraitsT>
    {
    public:
        DebugOStream() : std::basic_ostream<CharT, TraitsT>(new DebugStreamBuf<CharT, TraitsT>()) {}
        ~DebugOStream() {
            delete this->rdbuf();
        }
    };
}