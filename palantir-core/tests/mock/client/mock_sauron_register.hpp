#pragma once

#include "mock/palantir_mock.hpp"
#include "client/sauron_register.hpp"
#include "sauron/client/SauronClient.hpp"

namespace palantir::test {

class MockSauronRegister : public palantir::client::SauronRegister, public PalantirMock {
public:
    MockSauronRegister(const std::shared_ptr<sauron::client::SauronClient>& sauronClient) : SauronRegister(sauronClient) {}
    ~MockSauronRegister() override = default;	
    MOCK_METHOD(std::shared_ptr<sauron::client::SauronClient>, getSauronClient, (), (const, override));

};

} // namespace palantir::test 