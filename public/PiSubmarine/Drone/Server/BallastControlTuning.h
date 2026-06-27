#pragma once

#include "PiSubmarine/NormalizedFraction.h"

namespace PiSubmarine::Drone::Server
{
    struct BallastControlTuning
    {
        double ProportionalGain = 20.0;
        double IntegralGainPerSecond = 0.25;
        double IntegralLimit = 1.0;
        NormalizedFraction PositionDeadband = NormalizedFraction{0.01};
        NormalizedFraction MaxDutyCycle = NormalizedFraction{1.0};
    };
}
