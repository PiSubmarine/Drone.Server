 #pragma once

#include <chrono>
#include <cstddef>
#include <optional>

#include "PiSubmarine/Control/Gimbal/Servo/Config.h"
#include "PiSubmarine/Drone/Server/BallastControlTuning.h"
#include "PiSubmarine/Drone/Server/VerticalControlTuning.h"
#include "PiSubmarine/Grpc/Server/Server.h"
#include "PiSubmarine/Udp/Api/Endpoint.h"
#include "PiSubmarine/Video/Server/GStreamer/Config.h"
#include "PiSubmarine/Video/Subscription/Api/Endpoint.h"

namespace PiSubmarine::Drone::Server
{
    struct Config
    {
        ::PiSubmarine::Grpc::Server::TlsConfig GrpcServer;
        Video::Server::GStreamer::Config VideoController;
        BallastControlTuning BallastControl;
        VerticalControlTuning VerticalControl;
        Control::Gimbal::Servo::Config Gimbal;
        std::optional<Video::Subscription::Api::Endpoint> StartupVideoEndpoint;
        bool StartupVideoEnable = false;
        Udp::Api::Endpoint ControlEndpoint;
        Udp::Api::Endpoint TelemetryEndpoint;
        std::chrono::nanoseconds TickPeriod;
        std::size_t ReceiveQueueCapacity;
        std::size_t MaxDatagramSize;
    };
}
