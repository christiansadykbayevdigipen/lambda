#pragma once

#include "log/logger.h"
#include <memory>

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;
typedef signed long long int s64;

typedef float f32;
typedef double f64;

static_assert(sizeof(u8) == 1, "The size of an unsigned char must be exactly one byte");
static_assert(sizeof(u16) == 2, "The size of an unsigned short int must be exactly two bytes");
static_assert(sizeof(u32) == 4, "The size of an unsigned int must be exactly four bytes");
static_assert(sizeof(u64) == 8, "The size of an unsigned long long int must be exactly eight bytes");

static_assert(sizeof(s8) == 1, "The size of a signed char must be exactly one byte");
static_assert(sizeof(s16) == 2, "The size of a signed short int must be exactly two bytes");
static_assert(sizeof(s32) == 4, "The size of a signed int must be exactly four bytes");
static_assert(sizeof(s64) == 8, "The size of a signed long long int must be exactly eight bytes");

static_assert(sizeof(f32) == 4, "The size of a float must be exactly four bytes");
static_assert(sizeof(f64) == 8, "The size of a double must be exactly eight bytes");

#if defined(_WIN32) || defined(WIN32)
#define LAMBDA_PLATFORM_WINDOWS
#elif defined(__linux__)
#define LAMBDA_PLATFORM_LINUX
#elif defined(__APPLE__)
#define LAMBDA_PLATFORM_APPLE
#else
#error Unkown platform to Lambda game engine
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define LAMBDA_DEBUG_
#else
#define LAMBDA_RELEASE
#endif


// Logging

#define LAMBDA_FATAL(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Fatal, X, ##__VA_ARGS__)
#define LAMBDA_ERROR(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Error, X, ##__VA_ARGS__)

#if defined(LAMBDA_DEBUG_)
#define LAMBDA_WARN(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Warning, X, ##__VA_ARGS__)
#define LAMBDA_INFO(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Info, X, ##__VA_ARGS__)
#define LAMBDA_DEBUG(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Debug, X, ##__VA_ARGS__)
#define LAMBDA_TRACE(X, ...) lambda::Log(lambda::LogLevel::LogLevel_Trace, X, ##__VA_ARGS__)
#define LAMBDA_AUTOTRACE() lambda::Log(lambda::LogLevel::LogLevel_Trace, __PRETTY_FUNCTION__)
#else
#define LAMBDA_WARN(X, ...)
#define LAMBDA_INFO(X, ...)
#define LAMBDA_DEBUG(X, ...)
#define LAMBDA_TRACE(X, ...)
#define LAMBDA_AUTOTRACE()
#endif

// Rendering Api
namespace lambda {
    enum RenderingAPI {
        RenderingAPI_Vulkan,
        RenderingAPI_OpenGL
    };
}