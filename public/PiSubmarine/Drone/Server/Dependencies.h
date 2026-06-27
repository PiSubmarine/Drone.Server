#pragma once

#include <functional>
#include <vector>

#include "PiSubmarine/Ballast/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Battery/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Control/Lamp/Api/IController.h"
#include "PiSubmarine/Depth/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Lamp/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Motor/Bidirectional/Api/IController.h"
#include "PiSubmarine/Motor/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Motor/Unidirectional/Api/IController.h"
#include "PiSubmarine/Proximity/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Servo/IController.h"
#include "PiSubmarine/Time/ITickable.h"

namespace PiSubmarine::Drone::Server
{
    struct Dependencies
    {
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
}
