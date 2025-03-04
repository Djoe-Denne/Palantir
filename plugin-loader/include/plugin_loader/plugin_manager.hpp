#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "plugin/iplugin.hpp"
#include "plugin_loader.hpp"
#include <filesystem>

namespace palantir::plugin {

class PluginManager {
public:
    const static std::vector<std::string> PLUGIN_EXTENSIONS;

    PluginManager() = default;
    ~PluginManager() = default;

    PluginManager(const PluginManager&) = delete;
    auto operator=(const PluginManager&) -> PluginManager& = delete;
    PluginManager(PluginManager&&) = delete;
    auto operator=(PluginManager&&) -> PluginManager& = delete;

    /**
     * @brief Load a plugin from the specified path
     * @param path Path to the plugin shared library
     * @return true if plugin loaded successfully, false otherwise
     */
    auto loadPlugin(const std::string& path) -> bool;

    /**
     * @brief Load all plugins from a directory
     * @param directory Directory containing plugin shared libraries
     * @return Number of plugins successfully loaded
     */
    auto loadPluginsFromDirectory(const std::string& directory) -> size_t;

    /**
     * @brief Unload a plugin by name
     * @param name Name of the plugin to unload
     * @return true if plugin was unloaded, false if not found
     */
    auto unloadPlugin(const std::string& name) -> bool;

    /**
     * @brief Get a plugin by name
     * @param name Name of the plugin to get
     * @return Pointer to the plugin, nullptr if not found
     */
    [[nodiscard]] auto getPlugin(const std::string& name) const -> IPlugin*;

    /**
     * @brief Get all loaded plugins
     * @return Vector of pointers to loaded plugins
     */
    [[nodiscard]] auto getLoadedPlugins() const -> std::vector<IPlugin*>;

    /**
     * @brief Initialize all loaded plugins
     * @return true if all plugins initialized successfully
     */
    auto initializeAll() -> bool;

    /**
     * @brief Setup all plugins from a directory
     * @param directory Directory containing plugin shared libraries
     * @return true if all plugins setup successfully
     */
    auto setupFromDirectory(const std::filesystem::path& pluginsDir) -> bool;

    /**
     * @brief Shutdown all loaded plugins
     */
    auto shutdownAll() -> void;

private:
    PluginLoader loader_;
    std::unordered_map<std::string, std::unique_ptr<IPlugin>> plugins_;
};

} // namespace palantir::plugin
