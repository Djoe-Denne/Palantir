#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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
    ResourceUtils(const ResourceUtils&) = delete;
    ResourceUtils(ResourceUtils&&) = delete;
    auto operator=(const ResourceUtils&) -> ResourceUtils& = delete;
    auto operator=(ResourceUtils&&) -> ResourceUtils& = delete;

    /**
     * @brief Get the singleton instance of ResourceUtils
     * @return Reference to the singleton instance
     */
    static auto getInstance() -> std::shared_ptr<ResourceUtils>;

    static auto setInstance(const std::shared_ptr<ResourceUtils>& instance) -> void;

    /**
     * @brief Load a JavaScript file from the resource directory
     * @param filename The name of the file to load
     * @return String containing the file content
     * @throws std::runtime_error if the file cannot be loaded
     */
    [[nodiscard]] auto loadJavaScript(const std::filesystem::path& filename) const -> std::string;

    /**
     * @brief Load all JavaScript files from a specific subdirectory
     * @param subdirectory The subdirectory within the resource directory
     * @return Vector of pairs containing filename and file content
     */
    [[nodiscard]] auto loadAllJavaScriptsFromDirectory(const std::filesystem::path& subdirectory) const
        -> std::vector<std::pair<std::string, std::string>>;

    /**
     * @brief Get the full path to the resource directory
     * @return Path to the resource directory
     */
    [[nodiscard]] auto getResourceDirectory() const -> std::filesystem::path;

protected:
    ResourceUtils();
    /**
     * @brief Initialize the resource directory path
     */
    void initializeResourceDirectory();

    /**
     * @brief Read a file and return its content as a string
     * @param filepath Path to the file
     * @return String containing the file content
     */
    [[nodiscard]] auto readFile(const std::filesystem::path& filepath) const -> std::string;
    // clang-format off
    /**
     * @brief Read all files from a directory and return their content as a vector of pairs
     * @param directory Path to the directory
     * @return Vector of pairs containing filename and file content
     */
    [[nodiscard]] auto readAllFilesFromDirectory(const std::filesystem::path& directory,
                                                 const std::string& extension) const
        -> std::vector<std::pair<std::string, std::string>>;

    /**
     * @brief Set the resource directory
     * @param path Path to the resource directory
     */
    auto setResourceDirectory(const std::filesystem::path& path) -> void;
private:
    // clang-format on
#pragma warning(push)
#pragma warning(disable : 4251)
    static std::shared_ptr<ResourceUtils> instance_;
    std::filesystem::path resourceDirectory_;
#pragma warning(pop)
};

}  // namespace palantir::utils