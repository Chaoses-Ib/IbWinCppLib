#pragma once
#include "Common.hpp"
#include <ostream>
#include <sstream>
#include <string>

namespace ib {
    /*
    * Reference:
    * Sven, Axelsson. Using an output stream for debugging. https://gist.github.com/comargo/f7fe244e539325cef0f66704b58dc35a
    */

    template <typename CharT = wchar, typename TraitsT = std::char_traits<CharT>>
    class DebugStreamBuf : public std::basic_stringbuf<CharT, TraitsT>
    {
    public:
        using StringT = std::basic_string<CharT, TraitsT>;
        using StringViewT = std::basic_string_view<CharT, TraitsT>;

        DebugStreamBuf();
        DebugStreamBuf(StringT&& line_prefix);
        DebugStreamBuf(const CharT* line_prefix);
        DebugStreamBuf(StringViewT line_prefix);

        virtual ~DebugStreamBuf();
    protected:
        int sync();
    private:
        const StringT line_prefix;
    };

    template <typename CharT = wchar, typename TraitsT = std::char_traits<CharT>>
    class DebugOStream : public std::basic_ostream<CharT, TraitsT>
    {
    public:
        using StringT = std::basic_string<CharT, TraitsT>;
        using StringViewT = std::basic_string_view<CharT, TraitsT>;

        DebugOStream();
        DebugOStream(StringT&& line_prefix);
        DebugOStream(const CharT* line_prefix);
        DebugOStream(StringViewT line_prefix);

        ~DebugOStream();
    };
    
}

#include "impl/Diagnostic.hpp"