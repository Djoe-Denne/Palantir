#pragma once

#include "sauron/client/SauronClient.hpp"
#include "core_export.hpp"
#include <memory>

namespace palantir::client {

class PALANTIR_CORE_API SauronRegister {
public:
    // Delete copy and move operations
    SauronRegister(const SauronRegister&) = delete;
    auto operator=(const SauronRegister&) -> SauronRegister& = delete;
    SauronRegister(SauronRegister&&) = delete;
    auto operator=(SauronRegister&&) -> SauronRegister& = delete;

    // Singleton instance accessor
    [[nodiscard]] static auto getInstance() -> std::shared_ptr<SauronRegister>;

    static auto setInstance(const std::shared_ptr<SauronRegister>& instance) -> void;
    
    // Client accessor
    [[nodiscard]] virtual auto getSauronClient() const -> std::shared_ptr<sauron::client::SauronClient>;
    
    // Destructor
    virtual ~SauronRegister();

protected:
    // Protected constructor for testing
    SauronRegister();
    SauronRegister(const std::shared_ptr<sauron::client::SauronClient>& sauronClient);
private:
#pragma warning(push)
#pragma warning(disable: 4251)
    // PIMPL
    class Impl;
    std::unique_ptr<Impl> pImpl;    
    static std::shared_ptr<SauronRegister> instance_;
#pragma warning(pop)
};

} // namespace palantir::client
