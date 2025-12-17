#pragma once
#include <string>

#define LOG_INFO(msg)  Logger::Log(LogLevel::Info, msg)
#define LOG_WARN(msg)  Logger::Log(LogLevel::Warning, msg)
#define LOG_ERROR(msg) Logger::Log(LogLevel::Error, msg)

#ifndef NDEBUG
#define LOG_DEBUG(msg) Logger::Log(LogLevel::Debug, msg)
#else
#define LOG_DEBUG(msg)
#endif


enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

class Logger {
public:
    static void Init();
    static void Shutdown();

    static void Log(LogLevel level, const std::string& message);
};
