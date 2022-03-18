#pragma once
#include "Common.hpp"
#include "Cpp/iterator.hpp"

namespace ib {
    /// @brief An iterator for FindWindowExW
    class FindWindowEx_i : public iterator<std::input_iterator_tag, HWND, size, void, void> {
        using iterator = FindWindowEx_i;

        HWND parent;
        HWND current;
        cwzstring classname;
        cwzstring title;
        constexpr FindWindowEx_i(HWND parent, HWND current, cwzstring classname, cwzstring title)
            : parent(parent), current(current), classname(classname), title(title) {}
    public:
        FindWindowEx_i(HWND parent = 0, cwzstring classname = nullptr, cwzstring title = nullptr)
            : FindWindowEx_i(parent, FindWindowExW(parent, 0, classname, title), classname, title) {}
        constexpr FindWindowEx_i(const iterator& i) : FindWindowEx_i(i.parent, i.current, i.classname, i.title) {}

        constexpr iterator begin() { return *this; }
        constexpr iterator end() { iterator tmp = *this; tmp.current = 0; return tmp; }

        constexpr value_type operator*() const { return current; }

        iterator& operator++() {
            current = FindWindowExW(parent, current, classname, title);
            return *this;
        }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend constexpr bool operator==(const iterator& a, const iterator& b) {
            return a.current == b.current;
            //return a.current == b.current && a.parent == b.parent
            //    && a.classname == b.classname && a.title == b.title;
        }
        friend constexpr bool operator!=(const iterator& a, const iterator& b) { return !(a == b); }
    };
}