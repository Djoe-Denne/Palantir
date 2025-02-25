#include "plugin_loader/plugin_loader.hpp"
#include <dlfcn.h>

namespace interview_cheater::plugin {

LibraryHandle PluginLoader::loadLibrary(const std::string& path) {
    return dlopen(path.c_str(), RTLD_LAZY);
}

void PluginLoader::unloadLibrary(LibraryHandle handle) {
    dlclose(handle);
}

void* PluginLoader::getSymbol(LibraryHandle handle, const std::string& symbol) {
    return dlsym(handle, symbol.c_str());
}

} // namespace interview_cheater::plugin 