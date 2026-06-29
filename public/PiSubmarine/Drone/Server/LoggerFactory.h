#pragma once

#include <string_view>

#include "PiSubmarine/Drone/Server/Logging.h"
#include "PiSubmarine/Logging/Api/IFactory.h"

namespace PiSubmarine::Drone::Server
{
    class LoggerFactory final : public Logging::Api::IFactory
    {
    public:
        explicit LoggerFactory(LoggingConfig config = {});

        [[nodiscard]] std::shared_ptr<spdlog::logger> CreateLogger(std::string_view name) override;

    private:
        LoggingConfig m_Config;
    };
}
