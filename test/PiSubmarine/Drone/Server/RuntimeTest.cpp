#include <gtest/gtest.h>

#include "PiSubmarine/Degrees.h"
#include "PiSubmarine/Control/Lamp/Api/Command.h"
#include "PiSubmarine/Drone/Server/ErrorCode.h"
#include "PiSubmarine/Drone/Server/Runtime.h"

namespace PiSubmarine::Drone::Server
{
    namespace
    {
        class BallastTelemetryProviderStub final : public Ballast::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<Ballast::Telemetry::Api::State> GetState() const override
            {
                return Ballast::Telemetry::Api::State{};
            }
        };

        class BatteryTelemetryProviderStub final : public Battery::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<Battery::Telemetry::Api::State> GetState() const override
            {
                return Battery::Telemetry::Api::State{};
            }
        };

        class BidirectionalMotorStub final
            : public Motor::Bidirectional::Api::IController
            , public Motor::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<void> SetPowered(const bool enabled) override
            {
                m_IsPowered = enabled;
                return {};
            }

            [[nodiscard]] Error::Api::Result<bool> IsPowered() const override
            {
                return m_IsPowered;
            }

            [[nodiscard]] Error::Api::Result<SignedNormalizedFraction> GetDutyCycle() const override
            {
                return m_DutyCycle;
            }

            [[nodiscard]] Error::Api::Result<void> SetDutyCycle(const SignedNormalizedFraction dutyCycle) override
            {
                m_DutyCycle = dutyCycle;
                return {};
            }

            [[nodiscard]] Error::Api::Result<NormalizedFraction> GetForwardMinimalEffectiveDutyCycle() const override
            {
                return NormalizedFraction{0.1};
            }

            [[nodiscard]] Error::Api::Result<NormalizedFraction> GetReverseMinimalEffectiveDutyCycle() const override
            {
                return NormalizedFraction{0.1};
            }

            [[nodiscard]] Error::Api::Result<Motor::Telemetry::Api::State> GetState() const override
            {
                return Motor::Telemetry::Api::State{};
            }

        private:
            bool m_IsPowered = false;
            SignedNormalizedFraction m_DutyCycle{0.0};
        };

        class UnidirectionalMotorStub final
            : public Motor::Unidirectional::Api::IController
            , public Motor::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<void> SetPowered(const bool enabled) override
            {
                m_IsPowered = enabled;
                return {};
            }

            [[nodiscard]] Error::Api::Result<bool> IsPowered() const override
            {
                return m_IsPowered;
            }

            [[nodiscard]] Error::Api::Result<NormalizedFraction> GetDutyCycle() const override
            {
                return m_DutyCycle;
            }

            [[nodiscard]] Error::Api::Result<void> SetDutyCycle(const NormalizedFraction dutyCycle) override
            {
                m_DutyCycle = dutyCycle;
                return {};
            }

            [[nodiscard]] Error::Api::Result<NormalizedFraction> GetMinimumEffectiveDutyCycle() const override
            {
                return NormalizedFraction{0.1};
            }

            [[nodiscard]] Error::Api::Result<Motor::Telemetry::Api::State> GetState() const override
            {
                return Motor::Telemetry::Api::State{};
            }

        private:
            bool m_IsPowered = false;
            NormalizedFraction m_DutyCycle{0.0};
        };

        class DepthTelemetryProviderStub final : public Depth::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<Depth::Telemetry::Api::State> GetState() const override
            {
                return Depth::Telemetry::Api::State{};
            }
        };

        class LampControllerStub final
            : public Control::Lamp::Api::IController
            , public Lamp::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<void> SetTarget(const Control::Lamp::Api::Command& target) override
            {
                m_Intensity = target.Intensity();
                return {};
            }

            [[nodiscard]] Error::Api::Result<Lamp::Telemetry::Api::Status> GetStatus() const override
            {
                return Lamp::Telemetry::Api::Status{.Intensity = m_Intensity};
            }

        private:
            NormalizedFraction m_Intensity{0.0};
        };

        class ProximityTelemetryProviderStub final : public Proximity::Telemetry::Api::IProvider
        {
        public:
            [[nodiscard]] Error::Api::Result<Proximity::Telemetry::Api::State> GetState() const override
            {
                return Proximity::Telemetry::Api::State{};
            }
        };

        class ServoControllerStub final : public Servo::IController
        {
        public:
            [[nodiscard]] Error::Api::Result<void> SetTargetAngle(const Radians angle) override
            {
                m_TargetAngle = angle;
                return {};
            }

            [[nodiscard]] Radians GetTargetAngle() const override
            {
                return m_TargetAngle;
            }

            [[nodiscard]] AngularSector GetAllowedTargetAngleSector() const override
            {
                return AngularSector{Radians{0.0}, Degrees{180.0}.ToRadians()};
            }

            [[nodiscard]] Error::Api::Result<void> SetEnabled(const bool isEnabled) override
            {
                m_IsEnabled = isEnabled;
                return {};
            }

            [[nodiscard]] Error::Api::Result<bool> IsEnabled() const override
            {
                return m_IsEnabled;
            }

        private:
            Radians m_TargetAngle{0.0};
            bool m_IsEnabled = false;
        };

        class TickableStub final : public Time::ITickable
        {
        public:
            void Tick(const std::chrono::nanoseconds&, const std::chrono::nanoseconds&) override
            {
            }
        };

        [[nodiscard]] Runtime::Dependencies MakeDependencies(
            BallastTelemetryProviderStub& ballastTelemetryProvider,
            BatteryTelemetryProviderStub& batteryTelemetryProvider,
            BidirectionalMotorStub& ballastMotor,
            UnidirectionalMotorStub& frontLeftMotor,
            UnidirectionalMotorStub& frontRightMotor,
            UnidirectionalMotorStub& rearLeftMotor,
            UnidirectionalMotorStub& rearRightMotor,
            DepthTelemetryProviderStub& depthTelemetryProvider,
            LampControllerStub& lampController,
            ProximityTelemetryProviderStub& proximityTelemetryProvider,
            ServoControllerStub& servoController,
            TickableStub& tickable)
        {
            return Runtime::Dependencies{
                .BallastTelemetryProvider = ballastTelemetryProvider,
                .BatteryTelemetryProvider = batteryTelemetryProvider,
                .BallastMotorController = ballastMotor,
                .BallastMotorTelemetryProvider = ballastMotor,
                .FrontLeftMotorController = frontLeftMotor,
                .FrontLeftMotorTelemetryProvider = frontLeftMotor,
                .FrontRightMotorController = frontRightMotor,
                .FrontRightMotorTelemetryProvider = frontRightMotor,
                .RearLeftMotorController = rearLeftMotor,
                .RearLeftMotorTelemetryProvider = rearLeftMotor,
                .RearRightMotorController = rearRightMotor,
                .RearRightMotorTelemetryProvider = rearRightMotor,
                .DepthTelemetryProvider = depthTelemetryProvider,
                .LampController = lampController,
                .LampTelemetryProvider = lampController,
                .ProximityTelemetryProvider = proximityTelemetryProvider,
                .ServoController = servoController,
                .PlatformTickables = {tickable}};
        }
    }

    TEST(RuntimeTest, IsNotRunningBeforeRun)
    {
        BallastTelemetryProviderStub ballastTelemetryProvider;
        BatteryTelemetryProviderStub batteryTelemetryProvider;
        BidirectionalMotorStub ballastMotor;
        UnidirectionalMotorStub frontLeftMotor;
        UnidirectionalMotorStub frontRightMotor;
        UnidirectionalMotorStub rearLeftMotor;
        UnidirectionalMotorStub rearRightMotor;
        DepthTelemetryProviderStub depthTelemetryProvider;
        LampControllerStub lampController;
        ProximityTelemetryProviderStub proximityTelemetryProvider;
        ServoControllerStub servoController;
        TickableStub tickable;

        Runtime runtime(
            Runtime::Config{},
            MakeDependencies(
                ballastTelemetryProvider,
                batteryTelemetryProvider,
                ballastMotor,
                frontLeftMotor,
                frontRightMotor,
                rearLeftMotor,
                rearRightMotor,
                depthTelemetryProvider,
                lampController,
                proximityTelemetryProvider,
                servoController,
                tickable));

        EXPECT_FALSE(runtime.IsRunning());
    }

    TEST(RuntimeTest, RunFailsWhenGrpcTlsConfigurationIsMissing)
    {
        BallastTelemetryProviderStub ballastTelemetryProvider;
        BatteryTelemetryProviderStub batteryTelemetryProvider;
        BidirectionalMotorStub ballastMotor;
        UnidirectionalMotorStub frontLeftMotor;
        UnidirectionalMotorStub frontRightMotor;
        UnidirectionalMotorStub rearLeftMotor;
        UnidirectionalMotorStub rearRightMotor;
        DepthTelemetryProviderStub depthTelemetryProvider;
        LampControllerStub lampController;
        ProximityTelemetryProviderStub proximityTelemetryProvider;
        ServoControllerStub servoController;
        TickableStub tickable;

        Runtime runtime(
            Runtime::Config{},
            MakeDependencies(
                ballastTelemetryProvider,
                batteryTelemetryProvider,
                ballastMotor,
                frontLeftMotor,
                frontRightMotor,
                rearLeftMotor,
                rearRightMotor,
                depthTelemetryProvider,
                lampController,
                proximityTelemetryProvider,
                servoController,
                tickable));

        const auto result = runtime.Run();

        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().Cause, make_error_code(ErrorCode::GrpcServerStartFailed));
        EXPECT_FALSE(runtime.IsRunning());
    }
}
