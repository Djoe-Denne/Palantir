#include "plugin_loader/plugin_manager.hpp"

namespace palantir::plugin {

    const std::vector<std::string> PluginManager::PLUGIN_EXTENSIONS = {".dylib", ".so"};

} // namespace palantir::plugin
