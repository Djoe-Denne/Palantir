#pragma once

#include <string>

#ifdef _WIN32
    #ifdef PALANTIR_CORE_EXPORTS
        #define PALANTIR_CORE_API __declspec(dllexport)
    #else
        #define PALANTIR_CORE_API __declspec(dllimport)
    #endif

    #define PLUGIN_API __declspec(dllexport)
#else
    #define PALANTIR_CORE_API __attribute__((visibility("default")))
    #define PLUGIN_API __attribute__((visibility("default")))
#endif

namespace interview_cheater::plugin {

/**
 * @brief Interface that all plugins must implement
 */
class PALANTIR_CORE_API IPlugin {
public:
    IPlugin() = default;
    virtual ~IPlugin() = default;

    IPlugin(const IPlugin&) = delete;
    auto operator=(const IPlugin&) -> IPlugin& = delete;
    IPlugin(IPlugin&&) = delete;
    auto operator=(IPlugin&&) -> IPlugin& = delete;

    /**
     * @brief Initialize the plugin
     * @return true if initialization successful, false otherwise
     */
    virtual auto initialize() -> bool = 0;

    /**
     * @brief Shutdown the plugin
     */
    virtual auto shutdown() -> void = 0;

    /**
     * @brief Get the name of the plugin
     * @return Plugin name
     */
    [[nodiscard]] virtual auto getName() const -> std::string = 0;

    /**
     * @brief Get the version of the plugin
     * @return Plugin version string
     */
    [[nodiscard]] virtual auto getVersion() const -> std::string = 0;
};

// Plugin creation/destruction function types
using CreatePluginFunc = PLUGIN_API IPlugin* (*)();
using DestroyPluginFunc = PLUGIN_API void (*)(IPlugin*);

} // namespace interview_cheater::plugin

// Macros to help with plugin implementation
#define IMPLEMENT_PLUGIN(PluginClass) \
    extern "C" { \
        PLUGIN_API auto createPlugin() -> interview_cheater::plugin::IPlugin* { \
            return new PluginClass(); \
        } \
        PLUGIN_API auto destroyPlugin(interview_cheater::plugin::IPlugin* plugin) -> void { \
            delete plugin; \
        } \
    } 