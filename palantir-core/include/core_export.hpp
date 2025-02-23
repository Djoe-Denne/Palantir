/**
 * @file core_export.hpp
 * @brief Defines export/import macros for the core library.
 *
 * This file contains the platform-specific export/import macro definitions
 * used for proper DLL/shared library symbol visibility.
 */

#ifndef INTERVIEW_CHEATER_CORE_EXPORT_HPP
#define INTERVIEW_CHEATER_CORE_EXPORT_HPP

#ifdef _WIN32
    #ifdef PALANTIR_CORE_EXPORTS
        #define PALANTIR_CORE_API __declspec(dllexport)
        // Disable warning about STL classes requiring DLL interface
        #pragma warning(disable: 4251)
    #else
        #define PALANTIR_CORE_API __declspec(dllimport)
    #endif
#else
    #define PALANTIR_CORE_API 
#endif

#endif  // INTERVIEW_CHEATER_CORE_EXPORT_HPP 