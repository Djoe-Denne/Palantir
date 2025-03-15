#pragma once

#include <memory>
#include <string>

#include "core_export.hpp"
#include "window/component/icontent_manager.hpp"
#include "window/component/message/resize/resize_message_mapper.hpp"
#include "window/component/message/resize/resize_message_vo.hpp"

namespace palantir::window::component::message::resize {

/**
 * ResizeStrategy - A concrete strategy implementation that uses a strongly typed VO
 * rather than raw JSON.
 */
class PALANTIR_CORE_API ResizeStrategy {
public:
    using VOType = ResizeMessageVO;
    using Mapper = ResizeMessageMapper;
    ResizeStrategy() = delete;
    ~ResizeStrategy() = default;

    ResizeStrategy(const ResizeStrategy&) = delete;
    auto operator=(const ResizeStrategy&) -> ResizeStrategy& = delete;
    ResizeStrategy(ResizeStrategy&&) = delete;
    auto operator=(ResizeStrategy&&) -> ResizeStrategy& = delete;

    /**
     * Constructor with the event type this strategy handles.
     *
     * @param eventType The event type string.
     */
    explicit ResizeStrategy(std::string eventType, const std::shared_ptr<IContentManager>& contentManager);

    /**
     * Execute the strategy using the strongly typed value object
     *
     * @param logMessage The typed log message value object
     */
    auto execute(const ResizeMessageVO& resizeMessage) -> void;

    /**
     * Get the event type this strategy handles.
     *
     * @return The event type string.
     */
    [[nodiscard]] auto getEventType() const -> const std::string&;

private:
#pragma warning(push)
#pragma warning(disable : 4251)
    std::string eventType_;
    std::shared_ptr<IContentManager> contentManager_;
#pragma warning(pop)
};

}  // namespace palantir::window::component::message::resize
