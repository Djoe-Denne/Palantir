#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include <memory>

#include "utils/resource_utils.hpp"
#include "exception/exceptions.hpp"

using namespace palantir::utils;
using namespace testing;

// Mock class to test private methods
class ResourceUtilsTestable : public ResourceUtils {
public:
    ResourceUtilsTestable() : ResourceUtils() {}
    
    // Expose private methods for testing
    using ResourceUtils::readFile;
    using ResourceUtils::readAllFilesFromDirectory;
    using ResourceUtils::initializeResourceDirectory;
    
    // Allow manual setting of resource directory for testing
    void setResourceDirectory(const std::filesystem::path& path) {
        ResourceUtils::setResourceDirectory(path);
    }
};

class ResourceUtilsTest : public Test {
protected:
    void SetUp() override {
        // Create a temporary test directory structure
        testDir = std::filesystem::temp_directory_path() / "palantir_test_resources";
        std::filesystem::create_directories(testDir);
        std::filesystem::create_directories(testDir / "js");
        
        // Create test files
        createTestFile(testDir / "js" / "test1.js", "console.log('test1');");
        createTestFile(testDir / "js" / "test2.js", "console.log('test2');");
        createTestFile(testDir / "js" / "not_js.txt", "This is not a JS file");
        
        // Set environment variable for testing
        _putenv_s("PALANTIR_RESOURCE_DIR", testDir.string().c_str());
        
        // Get instance after setting up environment
        utils = ResourceUtils::getInstance();
        
        // Create testable instance for private method testing
        testableUtils = std::make_shared<ResourceUtilsTestable>();
        testableUtils->setResourceDirectory(testDir);
    }

    void TearDown() override {
        ResourceUtils::setInstance(nullptr);
        std::filesystem::remove_all(testDir);
        _putenv_s("PALANTIR_RESOURCE_DIR", "");
    }

    void createTestFile(const std::filesystem::path& path, const std::string& content) {
        std::ofstream file(path);
        file << content;
        file.close();
    }

    std::filesystem::path testDir;
    std::shared_ptr<ResourceUtils> utils;
    std::shared_ptr<ResourceUtilsTestable> testableUtils;
};

TEST_F(ResourceUtilsTest, GetInstance_ReturnsSameInstance) {
    auto instance1 = ResourceUtils::getInstance();
    auto instance2 = ResourceUtils::getInstance();
    EXPECT_EQ(instance1, instance2);
}

TEST_F(ResourceUtilsTest, SetInstance_ChangesInstance) {
    auto originalInstance = ResourceUtils::getInstance();
    auto newInstance = std::shared_ptr<ResourceUtils>(new ResourceUtilsTestable());
    
    ResourceUtils::setInstance(newInstance);
    auto retrievedInstance = ResourceUtils::getInstance();
    
    EXPECT_EQ(retrievedInstance, newInstance);
    EXPECT_NE(retrievedInstance, originalInstance);
    
    // Reset to original state
    ResourceUtils::setInstance(originalInstance);
}

TEST_F(ResourceUtilsTest, GetResourceDirectory_ReturnsCorrectPath) {
    EXPECT_EQ(utils->getResourceDirectory(), testDir);
}

TEST_F(ResourceUtilsTest, InitializeResourceDirectory_UsesEnvironmentVariable) {
    // Already tested by GetResourceDirectory test, but adding explicit test
    auto resourceDir = utils->getResourceDirectory();
    EXPECT_EQ(resourceDir, testDir);
}

TEST_F(ResourceUtilsTest, InitializeResourceDirectory_FallbackToDefaultPath) {
    // Setup a new instance with no environment variable
    _putenv_s("PALANTIR_RESOURCE_DIR", "");
    ResourceUtils::setInstance(nullptr);
    
    auto noEnvInstance = ResourceUtils::getInstance();
    auto defaultPath = std::filesystem::path("../../resource");
    
    // Check if it falls back to the default path
    bool isEqualOrContains = (noEnvInstance->getResourceDirectory() == defaultPath) ||
                             (noEnvInstance->getResourceDirectory().string().find(defaultPath.string()) != std::string::npos);
    
    EXPECT_TRUE(isEqualOrContains);
    
    // Reset environment for other tests
    _putenv_s("PALANTIR_RESOURCE_DIR", testDir.string().c_str());
}

TEST_F(ResourceUtilsTest, LoadJavaScript_ValidFile_ReturnsContent) {
    auto content = utils->loadJavaScript(testDir / "js" / "test1.js");
    EXPECT_EQ(content, "console.log('test1');");
}

TEST_F(ResourceUtilsTest, LoadJavaScript_NonexistentFile_ThrowsException) {
    EXPECT_THROW(utils->loadJavaScript(testDir / "js" / "nonexistent.js"), 
                palantir::exception::TraceableResourceLoadingException);
}

TEST_F(ResourceUtilsTest, ReadFile_ValidFile_ReturnsContent) {
    auto content = testableUtils->readFile(testDir / "js" / "test1.js");
    EXPECT_EQ(content, "console.log('test1');");
}

TEST_F(ResourceUtilsTest, ReadFile_NonexistentFile_ThrowsException) {
    EXPECT_THROW(testableUtils->readFile(testDir / "js" / "nonexistent.js"), 
                palantir::exception::TraceableResourceLoadingException);
}

TEST_F(ResourceUtilsTest, LoadAllJavaScriptsFromDirectory_ValidDirectory_ReturnsAllJsFiles) {
    auto files = utils->loadAllJavaScriptsFromDirectory("js");
    
    EXPECT_EQ(files.size(), 2);
    
    // Sort results for consistent testing
    std::sort(files.begin(), files.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    EXPECT_EQ(files[0].first, "test1.js");
    EXPECT_EQ(files[0].second, "console.log('test1');");
    EXPECT_EQ(files[1].first, "test2.js");
    EXPECT_EQ(files[1].second, "console.log('test2');");
}

TEST_F(ResourceUtilsTest, LoadAllJavaScriptsFromDirectory_EmptyDirectory_ReturnsEmptyVector) {
    std::filesystem::create_directories(testDir / "empty");
    auto files = utils->loadAllJavaScriptsFromDirectory("empty");
    EXPECT_TRUE(files.empty());
}

TEST_F(ResourceUtilsTest, LoadAllJavaScriptsFromDirectory_NonexistentDirectory_ReturnsEmptyVector) {
    auto files = utils->loadAllJavaScriptsFromDirectory("nonexistent");
    EXPECT_TRUE(files.empty());
}

TEST_F(ResourceUtilsTest, LoadAllJavaScriptsFromDirectory_DirectoryWithNoJsFiles_ReturnsEmptyVector) {
    std::filesystem::create_directories(testDir / "no_js");
    createTestFile(testDir / "no_js" / "test.txt", "Not a JS file");
    
    auto files = utils->loadAllJavaScriptsFromDirectory("no_js");
    EXPECT_TRUE(files.empty());
}

TEST_F(ResourceUtilsTest, ReadAllFilesFromDirectory_ValidDirectory_ReturnsAllFilesWithExtension) {
    auto files = testableUtils->readAllFilesFromDirectory(testDir / "js", ".js");
    
    EXPECT_EQ(files.size(), 2);
    
    // Sort results for consistent testing
    std::sort(files.begin(), files.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    EXPECT_EQ(files[0].first, "test1.js");
    EXPECT_EQ(files[0].second, "console.log('test1');");
    EXPECT_EQ(files[1].first, "test2.js");
    EXPECT_EQ(files[1].second, "console.log('test2');");
}

TEST_F(ResourceUtilsTest, ReadAllFilesFromDirectory_InvalidDirectory_ReturnsEmptyVector) {
    // Non-existent directory
    auto files = testableUtils->readAllFilesFromDirectory(testDir / "nonexistent", ".js");
    EXPECT_TRUE(files.empty());
    
    // Not a directory (file path instead)
    files = testableUtils->readAllFilesFromDirectory(testDir / "js" / "test1.js", ".js");
    EXPECT_TRUE(files.empty());
} 