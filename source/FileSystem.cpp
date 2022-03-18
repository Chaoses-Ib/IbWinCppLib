#include <IbWinCpp/FileSystem.hpp>

namespace ib {
    std::wstring path_to_realpath(std::wstring_view path, bool perfer_dos) {
        HANDLE file = CreateFileW(
            path.data(),
            0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL
        );
        if (file == INVALID_HANDLE_VALUE)
            return {};

        wchar buf[MAX_PATH];
        DWORD len = GetFinalPathNameByHandleW(file, buf, static_cast<DWORD>(std::size(buf)), 0);

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

        return std::wstring(result);
    }
}