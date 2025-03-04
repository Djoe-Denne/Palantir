#include "command/send_sauron_request_command.hpp"
#include "client/sauron_register.hpp"
#include "sauron/client/SauronClient.hpp"
#include "sauron/dto/DTOs.hpp"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "nlohmann/json.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace palantir::command {

SendSauronRequestCommand::SendSauronRequestCommand(std::string prompt) : prompt_(prompt), app_(Application::getInstance()) {}  // NOLINT

auto SendSauronRequestCommand::useDebounce() -> bool {
    return false;
}

auto SendSauronRequestCommand::execute() -> void {
    DEBUG_LOG("Sending Sauron request...");
    DEBUG_LOG("Prompt: ", prompt_); 
    auto images = loadImagesFromFolder();
    DEBUG_LOG("Images: ", images.size());
    auto sauronClient = client::SauronRegister::getSauronClient();
    DEBUG_LOG("Sauron client: ", sauronClient);

    auto aiAlgorithmWithImageQuery = sauron::dto::AIQueryRequest(prompt_, sauron::dto::AIProvider::OPENAI, "gpt-4o");
    DEBUG_LOG("AI algorithm with image query: ", aiAlgorithmWithImageQuery.toJson().dump(4));
    for (const auto& image : images) {
        aiAlgorithmWithImageQuery.addImage(image);
    }
    std::string responseStr;
    try {
        auto response = sauronClient->queryAlgorithm(aiAlgorithmWithImageQuery);
        responseStr = response.toJson().dump(4);
        DEBUG_LOG("Response: ", responseStr);
    } catch (const std::exception& e) {
        DEBUG_LOG("Error: ", e.what());
        return;
    }

    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getFirstWindow()) {
        auto contentManager = window->getContentManager();
        if (contentManager) {
            contentManager->setRootContent(responseStr);
        } else {
            throw std::runtime_error("Content manager not found");
        }
    } else {
        throw std::runtime_error("Window not found");
    }
}

auto SendSauronRequestCommand::loadImagesFromFolder() const -> std::vector<std::string> {
    std::vector<std::string> images;
    for (const auto& file : std::filesystem::directory_iterator("./screenshot")) {
        // load image in base64 format
        std::ifstream file(file.path(), std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        std::string image_base64 = "data:image/png;base64," + utils::StringUtils::base64_encode(buffer);
        images.push_back(image_base64);
    }
    return images;
}

} // namespace palantir::command

