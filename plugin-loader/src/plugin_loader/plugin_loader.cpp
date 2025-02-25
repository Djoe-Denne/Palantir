#include "plugin_loader/plugin_loader.hpp"

namespace interview_cheater::plugin {

PluginLoader::~PluginLoader() {
    if (currentHandle_) {
        unloadLibrary(currentHandle_);
    }
}

auto PluginLoader::loadPlugin(const std::string& path) -> std::unique_ptr<IPlugin> {
    // Unload any previously loaded library
    if (currentHandle_) {
        unloadLibrary(currentHandle_);
        currentHandle_ = nullptr;
        createFunc_ = nullptr;
        destroyFunc_ = nullptr;
    }

    // Load the new library
    currentHandle_ = loadLibrary(path);
    if (!currentHandle_) {
        return nullptr;
    }

    // Get the creation and destruction functions
    createFunc_ = reinterpret_cast<CreatePluginFunc>(getSymbol(currentHandle_, "createPlugin"));
    destroyFunc_ = reinterpret_cast<DestroyPluginFunc>(getSymbol(currentHandle_, "destroyPlugin"));

    if (!createFunc_ || !destroyFunc_) {
        unloadLibrary(currentHandle_);
        currentHandle_ = nullptr;
        return nullptr;
    }

    // Create the plugin instance
    IPlugin* plugin = createFunc_();
    if (!plugin) {
        unloadLibrary(currentHandle_);
        currentHandle_ = nullptr;
        return nullptr;
    }

    return std::unique_ptr<IPlugin>(plugin);
}

auto PluginLoader::unloadPlugin(IPlugin* plugin) -> void {
    if (plugin && destroyFunc_) {
        destroyFunc_(plugin);
    }
}

} // namespace interview_cheater::plugin 