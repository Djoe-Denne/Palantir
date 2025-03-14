#include "window/component/message/resize/resize_strategy.hpp"

#include "utils/logger.hpp"

namespace palantir::window::component::message::resize {

ResizeStrategy::ResizeStrategy(const std::string& eventType, std::shared_ptr<IContentManager> contentManager)
    : eventType_(eventType), contentManager_(contentManager) {}

auto ResizeStrategy::execute(const ResizeMessageVO& resizeMessage) -> void {
    DEBUG_LOG("ResizeStrategy handling event: ", eventType_, " with message: ", resizeMessage.width, "x",
              resizeMessage.height);
    contentManager_->resize(resizeMessage.width, resizeMessage.height);
}

auto ResizeStrategy::getEventType() const -> const std::string& { return eventType_; }

}  // namespace palantir::window::component::message::resize