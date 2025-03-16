#include "window/component/message/resize/resize_strategy.hpp"

#include "utils/logger.hpp"

namespace palantir::window::component::message::resize {

ResizeStrategy::ResizeStrategy(std::string eventType, const std::shared_ptr<IContentManager>& contentManager)
    : eventType_(std::move(eventType)), contentManager_(contentManager) {}

auto ResizeStrategy::execute(const ResizeMessageVO& resizeMessage) -> void {
    DebugLog("ResizeStrategy handling event: ", eventType_, " with message: ", resizeMessage.width, "x",
             resizeMessage.height);
    contentManager_->resize(resizeMessage.width, resizeMessage.height);
}

auto ResizeStrategy::getEventType() const -> const std::string& { return eventType_; }

}  // namespace palantir::window::component::message::resize