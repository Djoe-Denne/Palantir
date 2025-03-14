#pragma once

#include "window/component/message/resize/resize_message_vo.hpp"
#include "core_export.hpp"
#include <nlohmann/json.hpp>

namespace palantir::window::component::message::resize {

class PALANTIR_CORE_API ResizeMessageMapper {
public:
    inline static auto fromJson(const nlohmann::json& json) -> ResizeMessageVO {
        if (json.contains("width") && json.contains("height")) {
            return ResizeMessageVO{json["width"].get<int>(), json["height"].get<int>()};
        }
        throw std::invalid_argument("Invalid resize message");
    }
};

} // namespace palantir::window::component::message::resize

