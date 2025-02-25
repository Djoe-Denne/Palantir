#include "plugin_loader/plugin_loader.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace interview_cheater {
namespace plugin {

PluginLoader::PluginLoader() 
    : currentHandle_(nullptr)
    , createFunc_(nullptr)
    , destroyFunc_(nullptr) {
}

PluginLoader::~PluginLoader() {
    if (currentHandle_) {
        unloadLibrary(currentHandle_);
    }
}

std::unique_ptr<IPlugin> PluginLoader::loadPlugin(const std::string& path) {
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

void PluginLoader::unloadPlugin(IPlugin* plugin) {
    if (plugin && destroyFunc_) {
        destroyFunc_(plugin);
    }
}

LibraryHandle PluginLoader::loadLibrary(const std::string& path) {
#ifdef _WIN32
    return LoadLibraryA(path.c_str());
#else
    return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void PluginLoader::unloadLibrary(LibraryHandle handle) {
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

void* PluginLoader::getSymbol(LibraryHandle handle, const std::string& symbol) {
#ifdef _WIN32
    return reinterpret_cast<void*>(GetProcAddress(handle, symbol.c_str()));
#else
    return dlsym(handle, symbol.c_str());
#endif
}

} // namespace plugin
} // namespace interview_cheater 