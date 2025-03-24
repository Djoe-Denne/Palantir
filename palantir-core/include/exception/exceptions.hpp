#pragma once

#include "exception/base_exception.hpp"
#include "exception/config_exceptions.hpp"
#include "exception/input_exceptions.hpp"
#include "exception/plugin_exceptions.hpp"
#include "exception/traceable_exception.hpp"

namespace palantir::exception {

// Helper type aliases for common traceable exceptions
using TraceableConfigFileException = TraceableException<ConfigFileException>;
using TraceableContentManagerException = TraceableException<ContentManagerException>;
using TraceableInputFactoryException = TraceableException<InputFactoryException>;
using TraceableResourceLoadingException = TraceableException<ResourceLoadingException>;
using TraceablePluginInitializationException = TraceableException<PluginInitializationException>;
using TraceableUnknownCommandException = TraceableException<UnknownCommandException>;
using TraceableUIComponentNotFoundException = TraceableException<UIComponentNotFoundException>;
using TraceableShortcutConfigurationException = TraceableException<ShortcutConfigurationException>;
using TraceableNoCommandFoundException = TraceableException<NoCommandFoundException>;

}  // namespace palantir::exception