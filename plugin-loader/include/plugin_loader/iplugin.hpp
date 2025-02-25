#pragma once

#include <string>

#ifdef _WIN32
    #define PLUGIN_API __declspec(dllexport)
#else
    #define PLUGIN_API __attribute__((visibility("default")))
#endif

namespace interview_cheater {
namespace plugin {

/**
 * @brief Interface that all plugins must implement
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /**
     * @brief Initialize the plugin
     * @return true if initialization successful, false otherwise
     */
    virtual bool initialize() = 0;

    /**
     * @brief Shutdown the plugin
     */
    virtual void shutdown() = 0;

    /**
     * @brief Get the name of the plugin
     * @return Plugin name
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get the version of the plugin
     * @return Plugin version string
     */
    virtual std::string getVersion() const = 0;
};

// Plugin creation/destruction function types
using CreatePluginFunc = PLUGIN_API IPlugin* (*)();
using DestroyPluginFunc = PLUGIN_API void (*)(IPlugin*);

} // namespace plugin
} // namespace interview_cheater

// Macros to help with plugin implementation
#define IMPLEMENT_PLUGIN(PluginClass) \
    extern "C" { \
        PLUGIN_API interview_cheater::plugin::IPlugin* createPlugin() { \
            return new PluginClass(); \
        } \
        PLUGIN_API void destroyPlugin(interview_cheater::plugin::IPlugin* plugin) { \
            delete plugin; \
        } \
    } 