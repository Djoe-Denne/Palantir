#pragma once

#include <string>
#include <memory>
#include "plugin/iplugin.hpp"

#ifdef _WIN32
    #include <windows.h>
    using LibraryHandle = HMODULE;
#else
    using LibraryHandle = void*;
#endif

namespace interview_cheater {
namespace plugin {

class PluginLoader {
public:
    PluginLoader();
    ~PluginLoader();

    /**
     * @brief Load a plugin from the specified path
     * @param path Path to the plugin shared library
     * @return Pointer to the loaded plugin, nullptr if loading failed
     */
    std::unique_ptr<IPlugin> loadPlugin(const std::string& path);

    /**
     * @brief Unload a plugin
     * @param plugin Plugin to unload
     */
    void unloadPlugin(IPlugin* plugin);

private:
    LibraryHandle loadLibrary(const std::string& path);
    void unloadLibrary(LibraryHandle handle);
    void* getSymbol(LibraryHandle handle, const std::string& symbol);

    LibraryHandle currentHandle_;
    CreatePluginFunc createFunc_;
    DestroyPluginFunc destroyFunc_;
};
}
} 