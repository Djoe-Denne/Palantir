#pragma once

#include "sauron/client/SauronClient.hpp"
#include "core_export.hpp"

namespace interview_cheater::client {
class PALANTIR_CORE_API SauronRegister {
public:
    SauronRegister() = delete;
    ~SauronRegister() = delete;

    SauronRegister(const SauronRegister&) = delete;
    auto operator=(const SauronRegister&) -> SauronRegister& = delete;
    SauronRegister(SauronRegister&&) = delete;
    auto operator=(SauronRegister&&) -> SauronRegister& = delete;

    [[nodiscard]] static auto getSauronClient() -> std::shared_ptr<sauron::client::SauronClient>;
};
} // namespace interview_cheater::client
