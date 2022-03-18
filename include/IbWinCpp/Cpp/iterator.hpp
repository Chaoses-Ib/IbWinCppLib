#pragma once
#include "common.hpp"

namespace ib {
    /// @brief Alternative std::iterator
    template<typename TCategory, typename TValue, typename TDiff = ptrdiff, typename TPointer = TValue*, typename TReference = TValue&>
    class iterator {
    public:
        using iterator_category = TCategory;
        using value_type = TValue;
        using difference_type = TDiff;
        using pointer = TPointer;
        using reference = TReference;
    };
}