#pragma once
#include "Common.hpp"
#include "System.Memory.hpp"

namespace ib {
    struct Module {
        union {
            HMODULE handle;
            Addr base;
        };

        Module(HMODULE handle) : handle(handle) {};
        Module() : Module(NULL) {}

        // For operator bool and operator<=>
        operator void* () {
            return (void*)handle;
        }

        wzstring GetPath() {
            wzstring pathbuf = wzstring::New(MAX_PATH);
            if (!GetModuleFileNameW(handle, pathbuf, MAX_PATH))
                pathbuf.Delete();
            return pathbuf;
        }

        bool Free() {
            return FreeLibrary(handle);
        }
    };

    class ModuleFactory {
    public:
        // Doesn't increase reference count
        static Module Find(cwzstring module_name) {
            HMODULE h;
            GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, module_name, &h);
            return h;
        }

        static Module Load(cwzstring module_name) {
            return LoadLibraryW(module_name);
        }

        static Module CurrentProcess() {
            HMODULE h;
            GetModuleHandleExW(0, nullptr, &h);
            return h;
        }
    };
}