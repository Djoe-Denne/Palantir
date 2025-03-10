#include "client/sauron_register.hpp"

#include "sauron/client/SauronClient.hpp"
#include "sauron/client/http_client_curl.hpp"

namespace palantir::client {

std::shared_ptr<SauronRegister> SauronRegister::instance_;
// Implementation class (PIMPL)
class SauronRegister::Impl {
public:
    Impl()  // NOLINT
        : sauronClient(std::make_shared<sauron::client::SauronClient>(
              std::make_unique<sauron::client::HttpClientCurl>("http://localhost:3000"))) {
        sauronClient->login(sauron::dto::LoginRequest(
            "sk-proj-******", sauron::dto::AIProvider::OPENAI));  // put that outside the constructor
    }

    Impl(const Impl& other) = delete;
    auto operator=(const Impl& other) -> Impl& = delete;
    Impl(Impl&& other) = delete;
    auto operator=(Impl&& other) -> Impl& = delete;

    ~Impl() = default;

    explicit Impl(const std::shared_ptr<sauron::client::SauronClient>& sauronClient) : sauronClient(sauronClient) {}

    std::shared_ptr<sauron::client::SauronClient> sauronClient;
};

// Singleton instance
auto SauronRegister::getInstance() -> std::shared_ptr<SauronRegister> {
    if (!instance_) {
        instance_ = std::shared_ptr<SauronRegister>(new SauronRegister());
    }
    return instance_;
}

auto SauronRegister::setInstance(const std::shared_ptr<SauronRegister>& instance) -> void { instance_ = instance; }

// Constructor
SauronRegister::SauronRegister() = default;

SauronRegister::SauronRegister(const std::shared_ptr<sauron::client::SauronClient>& sauronClient)
    : pImpl_(std::make_unique<Impl>(sauronClient)) {}

// Destructor
SauronRegister::~SauronRegister() = default;

// Client accessor
auto SauronRegister::getSauronClient() const -> std::shared_ptr<sauron::client::SauronClient> {
    return pImpl_->sauronClient;
}

}  // namespace palantir::client
