#pragma once

#include <string>
#include <print>
#include <iostream>

namespace lambda {
    enum LogLevel {
        LogLevel_Fatal,
        LogLevel_Error,
        LogLevel_Warning,
        LogLevel_Info,
        LogLevel_Debug,
        LogLevel_Trace
    };

    template <typename ... Args>
    void Log(LogLevel level, const std::string& message, Args... args)
    {
        std::string prepend = "Unknown: ";

        switch(level) {
        case LogLevel::LogLevel_Fatal:
            prepend = "Fatal: ";
            break;
        case LogLevel::LogLevel_Error:
            prepend = "Error: ";
            break;
        case LogLevel::LogLevel_Warning:
            prepend = "Warning: ";
            break;
        case LogLevel::LogLevel_Info:
            prepend = "Info: ";
            break;
        case LogLevel::LogLevel_Debug:
            prepend = "Debug: ";
            break;
        case LogLevel::LogLevel_Trace:
            prepend = "Trace: ";
            break;
        default:
            std::println("Invalid log level was given to lambda::Log");
            break;
        }

        std::cout << prepend << message;

        ((std::cout << std::forward<Args>(args)), ...);
        
        std::cout << "\n";

        if(level == LogLevel::LogLevel_Fatal)
            throw std::runtime_error("lambda ran into an unrecoverable error");
    }
}