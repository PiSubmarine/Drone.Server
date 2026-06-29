#include "PiSubmarine/Drone/Server/LoggerFactory.h"

#include <format>
#include <mutex>
#include <string>
#include <vector>

#include <spdlog/logger.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "PiSubmarine/Drone/Server/Logging.h"

namespace PiSubmarine::Drone::Server
{
    namespace
    {
        constexpr std::size_t DefaultLoggerNameWidth = 32;

        std::mutex SharedLogFormattingMutex;
        std::size_t SharedLoggerNameWidth = DefaultLoggerNameWidth;
        std::weak_ptr<spdlog::sinks::sink> SharedTerminalSink;

        [[nodiscard]] std::string MakeLogPattern(const std::size_t loggerNameWidth)
        {
            return std::format("[%Y-%m-%d %T.%e] [%-{}n] [%^%-8l%$] [%s:%#] %v", loggerNameWidth);
        }

        [[nodiscard]] std::vector<spdlog::sink_ptr> GetSharedSinks()
        {
            std::scoped_lock lock(SharedLogFormattingMutex);

            if (const auto sink = SharedTerminalSink.lock())
            {
                return {sink};
            }

            auto sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
            sink->set_pattern(MakeLogPattern(SharedLoggerNameWidth));
            SharedTerminalSink = sink;
            return {sink};
        }

        void EnsureSinkPatternsFitName(const std::vector<spdlog::sink_ptr>& sinks, const std::string_view name)
        {
            std::scoped_lock lock(SharedLogFormattingMutex);

            if (name.size() <= SharedLoggerNameWidth)
            {
                return;
            }

            SharedLoggerNameWidth = name.size();
            const auto pattern = MakeLogPattern(SharedLoggerNameWidth);
            for (const auto& sink : sinks)
            {
                sink->set_pattern(pattern);
            }
        }
    }

    std::shared_ptr<spdlog::logger> CreateConfiguredLogger(const std::string_view name, const LoggingConfig& config)
    {
        const auto sinks = GetSharedSinks();
        EnsureSinkPatternsFitName(sinks, name);

        auto duplicateFilterSink =
            std::make_shared<spdlog::sinks::dup_filter_sink_mt>(config.DuplicateFilterMaxSkipDuration);
        for (const auto& sink : sinks)
        {
            duplicateFilterSink->add_sink(sink);
        }

        return std::make_shared<spdlog::logger>(std::string(name), std::move(duplicateFilterSink));
    }

    LoggerFactory::LoggerFactory(LoggingConfig config)
        : m_Config(std::move(config))
    {
    }

    std::shared_ptr<spdlog::logger> LoggerFactory::CreateLogger(const std::string_view name)
    {
        return CreateConfiguredLogger(name, m_Config);
    }
}
