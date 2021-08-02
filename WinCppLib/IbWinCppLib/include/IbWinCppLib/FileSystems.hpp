#pragma once
#include "Common.hpp"
#include <filesystem>

namespace ib {
    /// <summary>
    /// 
    /// </summary>
    /// <param name="path"></param>
    /// <param name="perfer_dos">The result is perferred in DOS path format (C:\Path) to device path format (\\?\C:\Path).</param>
    /// <returns></returns>
    inline wstring get_realpath(std::wstring_view path, bool perfer_dos = true) {
        HANDLE file = CreateFileW(
            path.data(),
            0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL
        );
        if (file == INVALID_HANDLE_VALUE)
            return {};

        wchar buf[MAX_PATH];
        DWORD len = GetFinalPathNameByHandleW(file, buf, std::size(buf), 0);
        
        std::wstring_view result;
        std::unique_ptr<wchar[]> buf2;
        if (len <= std::size(buf)) {
            result = buf;
        }
        else {
            buf2 = std::make_unique<wchar[]>(len);
            GetFinalPathNameByHandleW(file, buf2.get(), len, 0);
            result = buf2.get();
        }
        CloseHandle(file);

        if (perfer_dos) {
#if __cpp_lib_starts_ends_with
            if (result.starts_with(LR"(\\?\)"))
#else
            if (result.rfind(LR"(\\?\)", 0) == 0)
#endif
                result = result.substr(4);

        }

        return wstring(result);
    }
}