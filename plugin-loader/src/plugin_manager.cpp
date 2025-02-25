#include "plugin_loader/plugin_manager.hpp"
#include <filesystem>
#include <algorithm>

namespace interview_cheater {
namespace plugin {

PluginManager::PluginManager() = default;

PluginManager::~PluginManager() {
    shutdownAll();
}

bool PluginManager::loadPlugin(const std::string& path) {
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

size_t PluginManager::loadPluginsFromDirectory(const std::string& directory) {
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
#ifdef _WIN32
        if (path.extension() != ".dll") {
            continue;
        }
#else
        if (path.extension() != ".so" && path.extension() != ".dylib") {
            continue;
        }
#endif

        if (loadPlugin(path.string())) {
            ++loadedCount;
        }
    }

    return loadedCount;
}

bool PluginManager::unloadPlugin(const std::string& name) {
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        return false;
    }

    it->second->shutdown();
    loader_.unloadPlugin(it->second.get());
    plugins_.erase(it);
    return true;
}

IPlugin* PluginManager::getPlugin(const std::string& name) const {
    auto it = plugins_.find(name);
    return (it != plugins_.end()) ? it->second.get() : nullptr;
}

std::vector<IPlugin*> PluginManager::getLoadedPlugins() const {
    std::vector<IPlugin*> result;
    result.reserve(plugins_.size());
    
    for (const auto& pair : plugins_) {
        result.push_back(pair.second.get());
    }
    
    return result;
}

bool PluginManager::initializeAll() {
    bool success = true;
    for (auto& pair : plugins_) {
        if (!pair.second->initialize()) {
            success = false;
        }
    }
    return success;
}

void PluginManager::shutdownAll() {
    for (auto& pair : plugins_) {
        pair.second->shutdown();
        loader_.unloadPlugin(pair.second.get());
    }
    plugins_.clear();
}

} // namespace plugin
} // namespace palantir 