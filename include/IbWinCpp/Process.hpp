#pragma once
#include "Common.hpp"
#include "Memory.hpp"

namespace ib {
    struct Module {
        union {
            HMODULE handle;
            Addr base;
        };

        Module();
        Module(HMODULE handle);

        /// @brief For operator bool and operator<=>
        operator void*();

        /// Remember to delete the path
        wzstring get_path();

        bool free();
    };

    class ModuleFactory {
    public:
        /// Doesn't increase reference count
        static Module find(wzstring module_name);

        static Module load(wzstring module_name);

        static Module current_process();
    };
}