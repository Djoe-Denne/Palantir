#ifdef _WIN32
    #ifdef COMMANDS_PLUGIN_EXPORTS
        #define COMMANDS_PLUGIN_API __declspec(dllexport)
    #else
        #define COMMANDS_PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define COMMANDS_PLUGIN_API
#endif