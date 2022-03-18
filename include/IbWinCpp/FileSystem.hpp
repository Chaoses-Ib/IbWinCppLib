#pragma once
#include "Common.hpp"
#include <filesystem>

namespace ib {
    /// @param path 
    /// @param perfer_dos The result is perferred in DOS path format (C:\\Path) to device path format (\\\\?\\C:\\Path).
    std::wstring path_to_realpath(std::wstring_view path, bool perfer_dos = true);
}