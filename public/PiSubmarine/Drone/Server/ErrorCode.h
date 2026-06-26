#pragma once

#include <system_error>

namespace PiSubmarine::Drone::Server
{
    enum class ErrorCode
    {
        AlreadyStarted = 1,
        ControlBindFailed,
        TelemetryBindFailed,
        GrpcServerStartFailed,
        StartupVideoSubscriptionFailed
    };

    [[nodiscard]] std::error_code make_error_code(ErrorCode errorCode) noexcept;
}

namespace std
{
    template<>
    struct is_error_code_enum<PiSubmarine::Drone::Server::ErrorCode> : true_type
    {
    };
}
