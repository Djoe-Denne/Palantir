#pragma once

#include "mock/palantir_mock.hpp"
#include "sauron/client/SauronClient.hpp"
#include "sauron/dto/DTOs.hpp"

namespace palantir::test {

class MockSauronClient : public sauron::client::SauronClient, public PalantirMock {
public:
    MockSauronClient() : SauronClient(nullptr) {}
    ~MockSauronClient() override = default;

    MOCK_METHOD(sauron::dto::TokenResponse, login, (const sauron::dto::LoginRequest& request), (override));
    MOCK_METHOD(sauron::dto::AIAlgorithmResponse, queryAlgorithm, (const sauron::dto::AIQueryRequest& request), (override));
};

} // namespace palantir::test 