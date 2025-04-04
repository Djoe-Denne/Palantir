function(install_palantir_deps)
    message(STATUS "Checking or installing Palantir dependencies")

    install_nlohmann_json()
    install_sauron_sdk()
endfunction()

function(install_nlohmann_json)
    find_package(nlohmann_json QUIET)

    if(NOT nlohmann_json_SOURCE_DIR)
        message(STATUS "System nlohmann_json not found, building from source...")
        # Find or install nlohmann_json
        FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.11.2
        )
        FetchContent_MakeAvailable(nlohmann_json)

    else()
        # force nlohmann_json::nlohmann_json target 
        add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)
        set_target_properties(nlohmann_json::nlohmann_json PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${nlohmann_json_SOURCE_DIR}/include")

        message(STATUS "Found system nlohmann_json")
    endif()
endfunction()

function(install_sauron_sdk)
    # check if zip fil
    if (NOT SAURON_SDK_FOUND AND MAGIC_DEPS_INSTALL)
        set(GITHUB_OWNER "Djoe-Denne")
        set(GITHUB_REPO "Sauron-sdk")
        set(GITHUB_TAG "sauron-sdk-curl-latest")

        set(DEPS_DIR "${CMAKE_BINARY_DIR}/_deps")
        set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
        file(MAKE_DIRECTORY ${DEPS_DIR} ${BIN_DIR})

        set(ARTIFACT_NAME "sauron-sdk-curl-bin-${CMAKE_SYSTEM_NAME}-latest.zip")
        set(SOURCE_ARCHIVE "source.code.zip")
        
        set(ARTIFACT_URL "https://github.com/${GITHUB_OWNER}/${GITHUB_REPO}/releases/download/${GITHUB_TAG}/${ARTIFACT_NAME}")
        set(SOURCE_URL "https://github.com/${GITHUB_OWNER}/${GITHUB_REPO}/releases/download/${GITHUB_TAG}/${SOURCE_ARCHIVE}")
        
        set(AUTH_ARGS "")
        if(GITHUB_TOKEN)
            set(AUTH_ARGS "-H" "Authorization: token ${GITHUB_TOKEN}")
        endif()

        function(download_and_extract URL DEST EXTRACT_DEST)
            set(FILE_PATH "${DEPS_DIR}/${DEST}")
            message(STATUS "Downloading ${URL}")
            execute_process(COMMAND curl -L ${AUTH_ARGS} -o ${FILE_PATH} ${URL} RESULT_VARIABLE RESULT)
            if(NOT EXISTS ${FILE_PATH} OR RESULT)
                message(FATAL_ERROR "❌ Failed to download ${DEST}")
            endif()
            message(STATUS "✅ Downloaded ${FILE_PATH}")

            message(STATUS "Extracting ${DEST}")
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf ${FILE_PATH} WORKING_DIRECTORY ${EXTRACT_DEST} RESULT_VARIABLE EXTRACT_RESULT)
            if(EXTRACT_RESULT)
                message(FATAL_ERROR "❌ Failed to extract ${DEST}")
            endif()
            message(STATUS "✅ Extracted ${DEST} in ${EXTRACT_DEST}")
        endfunction()

        download_and_extract(${ARTIFACT_URL} ${ARTIFACT_NAME} ${BIN_DIR})
        download_and_extract(${SOURCE_URL} ${SOURCE_ARCHIVE} ${DEPS_DIR})

        set(SAURON_SDK_PATH "${DEPS_DIR}/api_spec/cpp-sdk" CACHE PATH "Path to Sauron SDK path")
        set(SAURON_SDK_CURL_PATH "${DEPS_DIR}/sauron-sdk-curl" CACHE PATH "Path to Sauron SDK curl path")
        file(COPY ${BIN_DIR}/sauron-sdk-curl.lib DESTINATION ${SAURON_SDK_CURL_PATH}/lib)
        set(SAURON_SDK_CURL_LIBRARY "${SAURON_SDK_CURL_PATH}/lib/sauron-sdk-curl.lib" CACHE FILEPATH "Path to Sauron SDK curl library")

        set(SAURON_SDK_INCLUDE_DIRS "${SAURON_SDK_PATH}/include" "${SAURON_SDK_CURL_PATH}/include" CACHE PATH "Path to Sauron SDK include directories")

        message(STATUS "✅ Sauron SDK installed at ${SAURON_SDK_CURL_PATH}")
    endif()

    message(STATUS "include dirs: ${SAURON_SDK_INCLUDE_DIRS}")

    add_library(sauron_sdk::curl UNKNOWN IMPORTED)
    set_target_properties(sauron_sdk::curl PROPERTIES
        IMPORTED_LOCATION "${SAURON_SDK_CURL_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SAURON_SDK_INCLUDE_DIRS}")

    set(SAURON_SDK_FOUND TRUE CACHE BOOL "Sauron SDK found")
    set(SAURON_SDK_LIBRARIES sauron_sdk::curl PARENT_SCOPE)

    message(STATUS "✅ Found system Sauron SDK: ${SAURON_SDK_LIBRARIES}")
endfunction()
