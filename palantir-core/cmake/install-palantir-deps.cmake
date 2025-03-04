function(install_palantir_deps)
    message(STATUS "Checking or installing palantir dependencies")
    find_package(Sauron-sdk QUIET)
    if(NOT SAURON_SDK_FOUND AND NOT TARGET sauron_sdk::curl AND MAGIC_DEPS_INSTALL)
    
        # Define the repository URL outside the if statement
        if(GITHUB_TOKEN)
            set(GIT_REPOSITORY https://${GITHUB_TOKEN}@github.com/Djoe-Denne/Sauron-sdk.git)
        else()
            set(GIT_REPOSITORY https://github.com/Djoe-Denne/Sauron-sdk.git)
        endif()

        message(STATUS "System Sauron SDK not found, building from source...")
        # Find or install Sauron SDK
        include(FetchContent)
        FetchContent_Declare(
            SAURON_SDK
            GIT_REPOSITORY ${GIT_REPOSITORY}
            GIT_BRANCH master
        )
        FetchContent_MakeAvailable(SAURON_SDK) 
    endif()
    if(NOT SAURON_SDK_FOUND)
        message(FATAL_ERROR "Sauron SDK not found")
    endif()
    message(STATUS "Found system Sauron SDK: ${SAURON_SDK_LIBRARIES}")

endfunction()
