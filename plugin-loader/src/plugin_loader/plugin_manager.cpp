#include "plugin_loader/plugin_manager.hpp"
#include <filesystem>
#include <algorithm>
#include <stdexcept>

namespace interview_cheater::plugin {

auto PluginManager::loadPlugin(const std::string& path) -> bool {
    auto plugin = loader_.loadPlugin(path);
    if (!plugin) {
        return false;
    }

    std::string name = plugin->getName();
    if (plugins_.find(name) != plugins_.end()) {
        // Plugin with this name already exists
        return false;
    }

    plugins_[name] = std::move(plugin);
    return true;
}

auto PluginManager::loadPluginsFromDirectory(const std::string& directory) -> size_t {
    size_t loadedCount = 0;
    const std::filesystem::path dir(directory);

    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
        return 0;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        const auto& path = entry.path();
        if (std::find(PLUGIN_EXTENSIONS.begin(), PLUGIN_EXTENSIONS.end(), path.extension()) == PLUGIN_EXTENSIONS.end()) {
            continue;
        }

        if (loadPlugin(path.string())) {
            ++loadedCount;
        }
    }

    return loadedCount;
}

auto PluginManager::unloadPlugin(const std::string& name) -> bool {
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        return false;
    }

    it->second->shutdown();
    loader_.unloadPlugin(it->second.get());
    plugins_.erase(it);
    return true;
}

auto PluginManager::getPlugin(const std::string& name) const -> IPlugin* {
    auto it = plugins_.find(name);
    return (it != plugins_.end()) ? it->second.get() : nullptr;
}

auto PluginManager::getLoadedPlugins() const -> std::vector<IPlugin*> {
    std::vector<IPlugin*> result;
    result.reserve(plugins_.size());
    
    for (const auto& pair : plugins_) {
        result.push_back(pair.second.get());
    }
    
    return result;
}

auto PluginManager::initializeAll() -> bool {
    bool success = true;
    for (auto& pair : plugins_) {
        if (!pair.second->initialize()) {
            success = false;
        }
    }
    return success;
}

auto PluginManager::setupFromDirectory(const std::filesystem::path& pluginsDir) -> bool {
        // Load all plugins from the plugins directory
        if (std::filesystem::exists(pluginsDir)) {
            size_t loadedCount = this->loadPluginsFromDirectory(pluginsDir.string());
            
            // Initialize all loaded plugins
            if (!this->initializeAll()) {
                throw std::runtime_error("Some plugins failed to initialize");
            }
        } else {
            throw std::runtime_error("Plugins directory not found: " + pluginsDir.string());
        }
        return true;
}

auto PluginManager::shutdownAll() -> void {
    for (auto& pair : plugins_) {
        pair.second->shutdown();
        loader_.unloadPlugin(pair.second.get());
    }
}

} // namespace interview_cheater::plugin 