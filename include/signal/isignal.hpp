#ifndef ISIGNAL_HPP
#define ISIGNAL_HPP

namespace interview_cheater::signal {

class ISignal {
public:
    virtual ~ISignal() = default;

    // Delete copy operations
    ISignal(const ISignal&) = delete;
    auto operator=(const ISignal&) -> ISignal& = delete;

    // Define move operations
    ISignal(ISignal&&) noexcept = default;
    auto operator=(ISignal&&) noexcept -> ISignal& = default;

    // Signal lifecycle methods
    virtual auto start() -> void = 0;
    virtual auto stop() -> void = 0;

    // Signal state
    [[nodiscard]] virtual auto isActive() const -> bool = 0;
    virtual auto check() -> void = 0;

protected:
    ISignal() = default;
};

}  // namespace interview_cheater::signal

#endif  // ISIGNAL_HPP