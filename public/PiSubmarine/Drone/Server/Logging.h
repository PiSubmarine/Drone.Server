#pragma once

#include <chrono>
#include <memory>
#include <string_view>

#include <spdlog/logger.h>

namespace PiSubmarine::Drone::Server
{
    struct LoggingConfig
    {
        std::chrono::milliseconds DuplicateFilterMaxSkipDuration = std::chrono::seconds(5);
    };

    [[nodiscard]] std::shared_ptr<spdlog::logger> CreateConfiguredLogger(
        std::string_view name,
        const LoggingConfig& config = {});
}
