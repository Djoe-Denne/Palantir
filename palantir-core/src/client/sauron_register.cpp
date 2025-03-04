#include "client/sauron_register.hpp"
#include "sauron/client/SauronClient.hpp"
#include "sauron/client/http_client_curl.hpp"

namespace palantir::client {
auto SauronRegister::getSauronClient() -> std::shared_ptr<sauron::client::SauronClient> {
    static auto sauronClient = std::make_shared<sauron::client::SauronClient>(std::make_unique<sauron::client::HttpClientCurl>("http://localhost:3000"));
    sauronClient->login(sauron::dto::LoginRequest("sk-proj-******", sauron::dto::AIProvider::OPENAI));
    return sauronClient;
}
} // namespace palantir::client
