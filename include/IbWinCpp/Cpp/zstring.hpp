#pragma once
#include <string>
#include "common.hpp"

namespace ib {
    /// @brief A simple wrapper for TChar*
    /// @note Could be nullptr
    template<typename TChar, typename TString>
    class basic_zstring {
    public:
        TChar* p;

        constexpr basic_zstring(const TChar* p) {
            this->p = const_cast<TChar*>(p);
        }
        constexpr basic_zstring(const TString& s) {
            p = const_cast<TChar*>(s.c_str());
        }

        constexpr static basic_zstring new_(size size) {
            return new TChar[size];
        }
        constexpr static basic_zstring new_(TString s) {
            auto p = new TChar[s.size() + 1];
            std::copy(s.begin(), s.end(), p);
            p[s.size()] = '\0';
            return p;
        }

        /// @brief delete and set to nullptr
        constexpr void delete_() {
            delete[] p;
            p = nullptr;
        }

        constexpr operator TChar*() {
            return p;
        }
        explicit constexpr operator TString() {
            return p;
        }

        constexpr operator void*() {
            return p;
        }
    };

    using wzstring = basic_zstring<wchar, std::wstring>;
}