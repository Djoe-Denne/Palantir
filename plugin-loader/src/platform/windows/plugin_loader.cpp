#include "plugin_loader/plugin_loader.hpp"
#include <windows.h>

namespace palantir::plugin {

LibraryHandle PluginLoader::loadLibrary(const std::string& path) {
    return LoadLibraryA(path.c_str());
}

void PluginLoader::unloadLibrary(LibraryHandle handle) {
    FreeLibrary(handle);
}

void* PluginLoader::getSymbol(LibraryHandle handle, const std::string& symbol) {
    return reinterpret_cast<void*>(GetProcAddress(handle, symbol.c_str()));
}

} // namespace palantir::plugin 