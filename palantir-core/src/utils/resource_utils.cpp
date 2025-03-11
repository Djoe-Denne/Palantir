#include "utils/resource_utils.hpp"
#include "utils/logger.hpp"

#include <cstdlib>

namespace palantir::utils {

// Initialize the static instance
std::shared_ptr<ResourceUtils> ResourceUtils::instance_ = nullptr;

ResourceUtils::ResourceUtils() {
    initializeResourceDirectory();
}

auto ResourceUtils::getInstance() -> std::shared_ptr<ResourceUtils> {
    if (!instance_) {
        instance_ = std::shared_ptr<ResourceUtils>(new ResourceUtils());
    }
    return instance_;
}

auto ResourceUtils::setInstance(std::shared_ptr<ResourceUtils> instance) -> void {
    instance_ = instance;
}

/**
 * @brief Load a JavaScript file from the resource directory
 * @param filename The name of the file to load
 * @return String containing the file content
 * @throws std::runtime_error if the file cannot be loaded
 */
auto ResourceUtils::loadJavaScript(const std::string& filename) const -> std::string {
    return readFile(filename);
}

/**
 * @brief Load all JavaScript files from a specific subdirectory
 * @param subdirectory The subdirectory within the resource directory
 * @return Vector of pairs containing filename and file content
 */
auto ResourceUtils::loadAllJavaScriptsFromDirectory(const std::string& subdirectory) const -> std::vector<std::pair<std::string, std::string>> {
    return readAllFilesFromDirectory(getResourceDirectory() / subdirectory, ".js");
}

/**
 * @brief Get the full path to the resource directory
 * @return Path to the resource directory
 */
auto ResourceUtils::getResourceDirectory() const -> std::filesystem::path {
    return resourceDirectory_;
}

void ResourceUtils::initializeResourceDirectory() {
    char* envResourceDir = nullptr;
    size_t len = 0;
    _dupenv_s(&envResourceDir, &len, "PALANTIR_RESOURCE_DIR");
    if (envResourceDir && std::filesystem::exists(envResourceDir)) {
        resourceDirectory_ = std::filesystem::path(envResourceDir);
        free(envResourceDir);
    } else {
        resourceDirectory_ = std::filesystem::path("../../resource");
    }

    DEBUG_LOG("Resource directory: ", resourceDirectory_.string());

}

auto ResourceUtils::readFile(const std::filesystem::path& filepath) const -> std::string {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        DEBUG_LOG("Failed to open file: ", filepath.string());
        throw std::runtime_error("Failed to open file: " + filepath.string());
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

auto ResourceUtils::readAllFilesFromDirectory(const std::filesystem::path& directory, const std::string& extension) const -> std::vector<std::pair<std::string, std::string>> {
    std::vector<std::pair<std::string, std::string>> files;
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        return files;
    }

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            DEBUG_LOG("Found file: ", entry.path().filename().string());
            files.emplace_back(entry.path().filename().string(), readFile(entry.path()));
        }
    }
    return files;
}


} // namespace palantir::utils 