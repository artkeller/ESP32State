/**
 * @file ESP32StateConfig.h
 * @brief Configurable output stream routing and logging for ESP32State.
 */

#ifndef ESP32STATE_CONFIG_H
#define ESP32STATE_CONFIG_H

#include <Arduino.h>

// Compile-time flag: strips all logging code when built with -DESP32STATE_DISABLE_LOGGING
#if defined(ESP32STATE_DISABLE_LOGGING)
    #define ESP32STATE_LOG(level, format, ...) ((void)0)
#else
    #define ESP32STATE_LOG(level, format, ...) ESP32State::log(level, format, ##__VA_ARGS__)
#endif

namespace ESP32State {

enum class LogLevel {
    NONE = 0,   ///< No output at all
    ERROR = 1,  ///< Errors only (panic, brownout, etc.)
    INFO = 2,   ///< Normal diagnostic data
    VERBOSE = 3 ///< Verbose logging
};

struct Config {
    Print* outputStream = &Serial;      ///< Target stream (&Serial, &Serial1, &File, or nullptr)
    LogLevel logLevel = LogLevel::INFO; ///< Default log level
    bool enablePrefix = true;           ///< Enable the "[ESP32State]" prefix
};

// Global library configuration
extern Config globalConfig;

/**
 * @brief Configures output routing at runtime.
 * @param stream Pointer to a Print object, or nullptr to mute output.
 * @param level Desired log level.
 */
inline void configure(Print* stream, LogLevel level = LogLevel::INFO) {
    globalConfig.outputStream = stream;
    globalConfig.logLevel = level;
}

/**
 * @brief Internal print helper with nullptr check and level filtering.
 */
inline void log(LogLevel level, const char* format, ...) {
    if (globalConfig.outputStream == nullptr || level > globalConfig.logLevel) {
        return; // nullptr or below the configured level -> abort before formatting
    }

    if (globalConfig.enablePrefix) {
        globalConfig.outputStream->print("[ESP32State] ");
    }

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    globalConfig.outputStream->println(buffer);
}

} // namespace ESP32State

#endif // ESP32STATE_CONFIG_H
