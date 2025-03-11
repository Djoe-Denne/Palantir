#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <memory>
#include "core_export.hpp"

namespace palantir::utils {

/**
 * @class ResourceUtils
 * @brief Utility class for loading and managing resources
 * 
 * This class provides functionality for loading resources (such as JavaScript files)
 * from the resource directory, ensuring they can be accessed by the application.
 */
class PALANTIR_CORE_API ResourceUtils {
public:
    ~ResourceUtils() = default;
    /**
     * @brief Get the singleton instance of ResourceUtils
     * @return Reference to the singleton instance
     */
    static auto getInstance() -> std::shared_ptr<ResourceUtils>;


    static auto setInstance(std::shared_ptr<ResourceUtils> instance) -> void;

    /**
     * @brief Load a JavaScript file from the resource directory
     * @param filename The name of the file to load
     * @return String containing the file content
     * @throws std::runtime_error if the file cannot be loaded
     */
    auto loadJavaScript(const std::string& filename) const -> std::string;

    /**
     * @brief Load all JavaScript files from a specific subdirectory
     * @param subdirectory The subdirectory within the resource directory
     * @return Vector of pairs containing filename and file content
     */
    auto loadAllJavaScriptsFromDirectory(const std::string& subdirectory) const -> std::vector<std::pair<std::string, std::string>>;

    /**
     * @brief Get the full path to the resource directory
     * @return Path to the resource directory
     */
    auto getResourceDirectory() const -> std::filesystem::path;

private:
    ResourceUtils();
    ResourceUtils(const ResourceUtils&) = delete;
    ResourceUtils(ResourceUtils&&) = delete;
    auto operator=(const ResourceUtils&) -> ResourceUtils& = delete;
    auto operator=(ResourceUtils&&) -> ResourceUtils& = delete;

#pragma warning(push)
#pragma warning(disable: 4251)
    static std::shared_ptr<ResourceUtils> instance_;
    std::filesystem::path resourceDirectory_;
#pragma warning(pop)
    
    /**
     * @brief Initialize the resource directory path
     */
    void initializeResourceDirectory();
    
    /**
     * @brief Read a file and return its content as a string
     * @param filepath Path to the file
     * @return String containing the file content
     */
    auto readFile(const std::filesystem::path& filepath) const -> std::string;

    /**
     * @brief Read all files from a directory and return their content as a vector of pairs
     * @param directory Path to the directory
     * @return Vector of pairs containing filename and file content
     */
    auto readAllFilesFromDirectory(const std::filesystem::path& directory, const std::string& extension) const -> std::vector<std::pair<std::string, std::string>>;
};

} // namespace palantir::utils 