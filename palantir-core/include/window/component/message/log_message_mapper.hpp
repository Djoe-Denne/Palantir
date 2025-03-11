#pragma once

#include "window/component/message/log_message_vo.hpp"
#include "core_export.hpp"
#include <nlohmann/json.hpp>

namespace palantir::window::component::message {

class PALANTIR_CORE_API LogMessageMapper {
public:
    inline static auto fromJson(const nlohmann::json& json) -> LogMessageVO {
        return LogMessageVO{json.dump()};
    }
};

} // namespace palantir::window::component::message

