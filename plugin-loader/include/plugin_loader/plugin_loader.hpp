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

namespace interview_cheater::plugin {

class PluginLoader {
public:
    PluginLoader();
    ~PluginLoader();

    /**
     * @brief Load a plugin from the specified path
     * @param path Path to the plugin shared library
     * @return Pointer to the loaded plugin, nullptr if loading failed
     */
    [[nodiscard]] auto loadPlugin(const std::string& path) -> std::unique_ptr<IPlugin>;

    /**
     * @brief Unload a plugin
     * @param plugin Plugin to unload
     */
    auto unloadPlugin(IPlugin* plugin) -> void;

private:
    auto loadLibrary(const std::string& path) -> LibraryHandle;
    auto unloadLibrary(LibraryHandle handle) -> void;
    auto getSymbol(LibraryHandle handle, const std::string& symbol) -> void*;

    LibraryHandle currentHandle_;
    CreatePluginFunc createFunc_;
    DestroyPluginFunc destroyFunc_;
};

} // namespace interview_cheater::plugin
