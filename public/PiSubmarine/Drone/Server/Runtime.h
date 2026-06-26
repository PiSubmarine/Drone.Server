#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "PiSubmarine/Ballast/Api/IController.h"
#include "PiSubmarine/Ballast/BallastFillFraction.h"
#include "PiSubmarine/Ballast/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Battery/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Control/Lamp/Api/IController.h"
#include "PiSubmarine/Depth/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Drone/Server/ErrorCode.h"
#include "PiSubmarine/Error/Api/Result.h"
#include "PiSubmarine/Grpc/Server/Server.h"
#include "PiSubmarine/Lamp/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Meters.h"
#include "PiSubmarine/Motor/Bidirectional/Api/IController.h"
#include "PiSubmarine/Motor/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Motor/Unidirectional/Api/IController.h"
#include "PiSubmarine/Proximity/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Servo/IController.h"
#include "PiSubmarine/Time/ITickable.h"
#include "PiSubmarine/Udp/Api/Endpoint.h"
#include "PiSubmarine/Video/Server/GStreamer/Config.h"
#include "PiSubmarine/Video/Subscription/Api/Endpoint.h"

namespace PiSubmarine::Drone::Server
{
    class Runtime
    {
    public:
        struct VerticalControlConfig
        {
            Ballast::BallastFillFraction InitialEquilibriumBallastFill =
                Ballast::BallastFillFraction{NormalizedFraction{0.5}};
        };

        struct Config
        {
            ::PiSubmarine::Grpc::Server::TlsConfig GrpcServer;
            Video::Server::GStreamer::Config VideoController;
            VerticalControlConfig VerticalControl;
            std::optional<Video::Subscription::Api::Endpoint> StartupVideoEndpoint;
            bool StartupVideoEnable = false;
            Udp::Api::Endpoint ControlEndpoint{"127.0.0.1", 50052};
            Udp::Api::Endpoint TelemetryEndpoint{"127.0.0.1", 50053};
            std::chrono::nanoseconds TickPeriod = std::chrono::milliseconds(10);
            std::size_t ReceiveQueueCapacity = 64;
            std::size_t MaxDatagramSize = 65507;
        };

        struct Dependencies
        {
            Ballast::Api::IController& BallastController;
            Ballast::Telemetry::Api::IProvider& BallastTelemetryProvider;
            Battery::Telemetry::Api::IProvider& BatteryTelemetryProvider;
            Motor::Bidirectional::Api::IController& BallastMotorController;
            Motor::Telemetry::Api::IProvider& BallastMotorTelemetryProvider;
            Motor::Unidirectional::Api::IController& FrontLeftMotorController;
            Motor::Telemetry::Api::IProvider& FrontLeftMotorTelemetryProvider;
            Motor::Unidirectional::Api::IController& FrontRightMotorController;
            Motor::Telemetry::Api::IProvider& FrontRightMotorTelemetryProvider;
            Motor::Unidirectional::Api::IController& RearLeftMotorController;
            Motor::Telemetry::Api::IProvider& RearLeftMotorTelemetryProvider;
            Motor::Unidirectional::Api::IController& RearRightMotorController;
            Motor::Telemetry::Api::IProvider& RearRightMotorTelemetryProvider;
            Depth::Telemetry::Api::IProvider& DepthTelemetryProvider;
            Control::Lamp::Api::IController& LampController;
            Lamp::Telemetry::Api::IProvider& LampTelemetryProvider;
            Proximity::Telemetry::Api::IProvider& ProximityTelemetryProvider;
            Servo::IController& ServoController;
            std::vector<std::reference_wrapper<Time::ITickable>> PlatformTickables;
        };

        Runtime(const Config& config, Dependencies dependencies);
        ~Runtime();

        [[nodiscard]] Error::Api::Result<void> Run();
        void Stop() noexcept;
        [[nodiscard]] bool IsRunning() const noexcept;

    private:
        [[nodiscard]] Error::Api::Result<void> StartServices();
        void StopServices();
        [[nodiscard]] static Error::Api::Error MakeRuntimeError(ErrorCode code) noexcept;
        static void ThrowIfError(const Error::Api::Result<void>& result, const char* action);

        class Impl;
        std::unique_ptr<Impl> m_Impl;
        bool m_IsRunning = false;
    };
}
