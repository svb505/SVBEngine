#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <format>

static std::ofstream logFile;

void Logger::Init() { logFile.open("engine_logs.log", std::ios::out | std::ios::app); }
void Logger::Shutdown() { if (logFile.is_open()) logFile.close(); }
static const char* LevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::Info:    return "[INFO]";
    case LogLevel::Warning: return "[WARN]";
    case LogLevel::Error:   return "[ERROR]";
    case LogLevel::Debug:   return "[DEBUG]";
    }
    return "[UNKNOWN]";
}
void Logger::Log(LogLevel level, const std::string& message) {
    std::time_t t = std::time(nullptr);

    std::tm tm{};
    localtime_s(&tm, &t);   

    char timeBuf[32];
    std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &tm);

    std::string final = std::format("{} {} {}", timeBuf, LevelToString(level), message);

    if (logFile.is_open()) logFile << final << std::endl;
}

