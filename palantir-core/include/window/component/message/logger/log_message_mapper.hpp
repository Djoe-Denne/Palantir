#pragma once

#include <nlohmann/json.hpp>

#include "core_export.hpp"
#include "window/component/message/logger/log_message_vo.hpp"

namespace palantir::window::component::message::logger {

class PALANTIR_CORE_API LogMessageMapper {
public:
    static auto fromJson(const nlohmann::json& json) -> LogMessageVO { return LogMessageVO{json.dump()}; }
};

}  // namespace palantir::window::component::message::logger
