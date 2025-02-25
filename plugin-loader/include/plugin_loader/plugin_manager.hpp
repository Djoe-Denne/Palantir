#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "iplugin.hpp"
#include "plugin_loader.hpp"

namespace interview_cheater {
namespace plugin {

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    /**
     * @brief Load a plugin from the specified path
     * @param path Path to the plugin shared library
     * @return true if plugin loaded successfully, false otherwise
     */
    bool loadPlugin(const std::string& path);

    /**
     * @brief Load all plugins from a directory
     * @param directory Directory containing plugin shared libraries
     * @return Number of plugins successfully loaded
     */
    size_t loadPluginsFromDirectory(const std::string& directory);

    /**
     * @brief Unload a plugin by name
     * @param name Name of the plugin to unload
     * @return true if plugin was unloaded, false if not found
     */
    bool unloadPlugin(const std::string& name);

    /**
     * @brief Get a plugin by name
     * @param name Name of the plugin to get
     * @return Pointer to the plugin, nullptr if not found
     */
    IPlugin* getPlugin(const std::string& name) const;

    /**
     * @brief Get all loaded plugins
     * @return Vector of pointers to loaded plugins
     */
    std::vector<IPlugin*> getLoadedPlugins() const;

    /**
     * @brief Initialize all loaded plugins
     * @return true if all plugins initialized successfully
     */
    bool initializeAll();

    /**
     * @brief Shutdown all loaded plugins
     */
    void shutdownAll();

private:
    PluginLoader loader_;
    std::unordered_map<std::string, std::unique_ptr<IPlugin>> plugins_;
};
}
} 