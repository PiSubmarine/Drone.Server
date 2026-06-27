#pragma once

#include "PiSubmarine/Ballast/BallastFillFraction.h"
#include "PiSubmarine/Meters.h"
#include "PiSubmarine/NormalizedFraction.h"

namespace PiSubmarine::Drone::Server
{
    struct VerticalControlTuning
    {
        double ProportionalGain = 0.5;
        double IntegralGainPerSecond = 0.5;
        double DerivativeGainSeconds = 0.5;
        double IntegralLimitMetersSeconds = 100.0;
        Meters DepthDeadband = 0.05_m;
        NormalizedFraction MaximumBallastCorrection = NormalizedFraction{0.5};
        Ballast::BallastFillFraction InitialEquilibriumBallastFill =
            Ballast::BallastFillFraction{NormalizedFraction{0.5}};
    };
}
