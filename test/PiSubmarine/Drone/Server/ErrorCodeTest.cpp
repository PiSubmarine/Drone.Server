#include <gtest/gtest.h>

#include "PiSubmarine/Drone/Server/ErrorCode.h"

namespace PiSubmarine::Drone::Server
{
    TEST(ErrorCodeTest, ProducesExpectedMessage)
    {
        EXPECT_EQ(make_error_code(ErrorCode::GrpcServerStartFailed).message(), "failed to start shared gRPC server");
    }
}
