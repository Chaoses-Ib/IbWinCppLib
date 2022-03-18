#include <IbWinCpp/Process.hpp>

namespace ib {
    Module::Module(HMODULE handle) : handle(handle) {};
    Module::Module() : Module(NULL) {}

    Module::operator void*() {
        return (void*)handle;
    }

    wzstring Module::get_path() {
        wzstring pathbuf = wzstring::new_(MAX_PATH);
        if (!GetModuleFileNameW(handle, pathbuf, MAX_PATH))
            pathbuf.delete_();
        return pathbuf;
    }

    bool Module::free() {
        return FreeLibrary(handle);
    }

    Module ModuleFactory::find(wzstring module_name) {
        HMODULE h;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, module_name, &h);
        return h;
    }

    Module ModuleFactory::load(wzstring module_name) {
        return LoadLibraryW(module_name);
    }

    Module ModuleFactory::current_process() {
        HMODULE h;
        GetModuleHandleExW(0, nullptr, &h);
        return h;
    }
}