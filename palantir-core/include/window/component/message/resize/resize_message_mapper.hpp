#pragma once

#include <nlohmann/json.hpp>

#include "core_export.hpp"
#include "window/component/message/resize/resize_message_vo.hpp"

namespace palantir::window::component::message::resize {

class PALANTIR_CORE_API ResizeMessageMapper {
public:
    static auto fromJson(const nlohmann::json& json) -> ResizeMessageVO {
        if (json.contains("width") && json.contains("height")) {
            return ResizeMessageVO{json["width"].get<int>(), json["height"].get<int>()};
        }
        throw std::invalid_argument("Invalid resize message");
    }
};

}  // namespace palantir::window::component::message::resize
