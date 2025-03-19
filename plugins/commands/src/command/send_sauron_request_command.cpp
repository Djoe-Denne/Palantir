#include "command/send_sauron_request_command.hpp"
#include "client/sauron_register.hpp"
#include "sauron/client/SauronClient.hpp"
#include "sauron/dto/DTOs.hpp"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "nlohmann/json.hpp"
#include "exception/exceptions.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace palantir::command {

SendSauronRequestCommand::SendSauronRequestCommand(const std::string& prompt) : prompt_(prompt) {}  // NOLINT

auto SendSauronRequestCommand::useDebounce() const -> bool {
    return false;
}

auto SendSauronRequestCommand::execute() const -> void {
    DebugLog("Sending Sauron request...");
    DebugLog("Prompt: ", prompt_); 
    auto images = loadImagesFromFolder();
    DebugLog("Images: ", images.size());
    auto sauronRegister = client::SauronRegister::getInstance();
    auto sauronClient = sauronRegister->getSauronClient();
    DebugLog("Sauron client: ", sauronClient);

    auto aiAlgorithmWithImageQuery = sauron::dto::AIQueryRequest(prompt_, sauron::dto::AIProvider::OPENAI, "gpt-4o");
    DebugLog("AI algorithm with image query: ", aiAlgorithmWithImageQuery.toJson().dump(4));
    for (const auto& image : images) {
        aiAlgorithmWithImageQuery.addImage(image);
    }
    std::string responseStr;
    try {
        auto response = sauronClient->queryAlgorithm(aiAlgorithmWithImageQuery);
        responseStr = response.toJson().dump(4);
        DebugLog("Response: ", responseStr);
    } catch (const std::exception& e) {
        DebugLog("Error: ", e.what());
        throw palantir::exception::TraceableException<palantir::exception::BaseException>("Failed to query AI algorithm");
    }

    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
        auto contentManager = window->getContentManager();
        if (contentManager) {
            contentManager->setRootContent(responseStr);
        } else {
            throw palantir::exception::TraceableContentManagerException("Content manager not found");
        }
    } else {
        throw palantir::exception::TraceableUIComponentNotFoundException("Window not found");
    }
}

auto SendSauronRequestCommand::loadImagesFromFolder() const -> std::vector<std::string> {
    std::vector<std::string> images;
    if (!std::filesystem::exists("./screenshot")) {
        return images;
    }
    for (const auto& file : std::filesystem::directory_iterator("./screenshot")) {
        // load image in base64 format
        std::ifstream fileStream(file.path(), std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(fileStream), {});
        std::string image_base64 = "data:image/png;base64," + utils::StringUtils::base64_encode(buffer);
        images.push_back(image_base64);
    }
    return images;
}

} // namespace palantir::command

