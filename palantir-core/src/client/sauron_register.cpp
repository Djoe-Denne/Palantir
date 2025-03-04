#include "client/sauron_register.hpp"
#include "sauron/client/SauronClient.hpp"
#include "client/http_client_curl.hpp"

namespace interview_cheater::client {
auto SauronRegister::getSauronClient() -> std::shared_ptr<sauron::client::SauronClient> {
    static auto sauronClient = std::make_shared<sauron::client::SauronClient>(std::make_unique<sauron::client::HttpClientCurl>("http://localhost:3000"));
    sauronClient->login(sauron::dto::LoginRequest("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", sauron::dto::AIProvider::OPENAI));
    return sauronClient;
}
} // namespace interview_cheater::client
