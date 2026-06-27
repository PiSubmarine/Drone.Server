#pragma once

#include <memory>

#include "PiSubmarine/Drone/Server/Config.h"
#include "PiSubmarine/Drone/Server/Dependencies.h"
#include "PiSubmarine/Drone/Server/ErrorCode.h"
#include "PiSubmarine/Error/Api/Result.h"

namespace PiSubmarine::Drone::Server
{
    class Runtime
    {
    public:
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
