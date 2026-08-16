/**
 * @file ESP32State.h
 * @brief Analyzer for ESP32 startup conditions and post-mortem state evaluation.
 *
 * Structural note (2026 rewrite): everything that previously lived in a
 * separate `namespace ESP32State { ... }` (in ESP32StateConfig.h) has
 * been merged into this single `class ESP32State` as nested types and
 * static members. Having both a `namespace ESP32State` and a
 * `class ESP32State` in the same translation unit is not valid C++ —
 * they occupy the same identifier scope — and the previous split
 * layout did not actually compile. This file is now self-contained;
 * ESP32StateConfig.h is kept only as a deprecated compatibility shim
 * (see that file).
 */

#ifndef ESP32STATE_H
#define ESP32STATE_H

// Framework + version + target capability detection, generated from
// real espressif/esp-idf and espressif/arduino-esp32 sources rather
// than hand-typed. See harvester/README.md in the project repository
// for the full method. Must be included before anything else here —
// ESP32STATE_FRAMEWORK_ARDUINO below depends on it.
#include "variants/ESP32State_Capabilities.h"

#if !ESP32STATE_FRAMEWORK_ARDUINO
#error "ESP32State currently requires the Arduino Print/Serial API (no " \
       "<Arduino.h> found via __has_include). This correctly covers both " \
       "the plain Arduino IDE and the \"Arduino as an ESP-IDF component\" " \
       "build path (espressif/arduino-esp32#3866) — unlike the previous " \
       "\"#ifdef ESP32\" guard, which did not detect the latter. Pure " \
       "ESP-IDF without the Arduino component is not yet supported."
#endif

#include <Arduino.h>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <vector>

#if ESP32STATE_HAS_IDF_VERSION
#include <esp_system.h>
#include <esp_sleep.h>
#endif

class ESP32State {
public:
    // ------------------------------------------------------------------
    // Condition / result types
    // ------------------------------------------------------------------
    using Condition = std::function<bool()>;
    using Callback = std::function<void()>;

    struct ConditionPair {
        Condition condition; ///< Condition to evaluate
        Callback callback;   ///< Callback to run on match
    };

    struct AnalysisResult {
        size_t matched;         ///< Number of conditions matched
        size_t unmatched;       ///< Number of conditions not matched
        size_t total_evaluated; ///< matched + unmatched, for convenience in log messages
    };

    // ------------------------------------------------------------------
    // Logging / output configuration
    // (previously a free-standing `namespace ESP32State` in
    // ESP32StateConfig.h — now nested members of this class instead,
    // which is what actually makes `ESP32State::configure(...)` and
    // `ESP32State analyzer(...)` both valid at the same time.)
    // ------------------------------------------------------------------
    enum class LogLevel {
        NONE = 0,   ///< No output at all
        ERROR = 1,  ///< Errors only (panic, brownout, etc.)
        WARN = 2,   ///< Warnings and errors
        INFO = 3,   ///< Normal diagnostic data
        VERBOSE = 4 ///< Verbose logging
    };

    struct Config {
        Print* outputStream = &Serial;      ///< Target stream (&Serial, &Serial1, &File, or nullptr)
        LogLevel logLevel = LogLevel::INFO; ///< Default log level
        bool enablePrefix = true;           ///< Enable the "[ESP32State]" prefix
    };

    /// Global library configuration. Defined once in ESP32State.cpp.
    static Config globalConfig;

    /**
     * @brief Configures output routing at runtime.
     * @param stream Pointer to a Print object, or nullptr to mute output.
     * @param level Desired log level.
     */
    static void configure(Print* stream, LogLevel level = LogLevel::INFO) {
        globalConfig.outputStream = stream;
        globalConfig.logLevel = level;
    }

    /**
     * @brief Internal print helper with nullptr check and level filtering.
     */
    static void log(LogLevel level, const char* format, ...) {
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

    // ------------------------------------------------------------------
    // Hardware Abstraction Layer
    // (previously `namespace ESP32State { namespace HAL { ... } }` in
    // ESP32State_HAL.h — same collision problem, same fix: a nested
    // struct with static methods instead of a nested namespace, since
    // C++ does not allow a namespace nested inside a class at all.)
    // ------------------------------------------------------------------
    struct HAL {
        /**
         * @brief Returns the exact SoC target name at compile-time.
         */
        static constexpr const char* getTargetName() {
#if defined(CONFIG_IDF_TARGET_ESP32)
            return "ESP32";
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
            return "ESP32-S2";
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
            return "ESP32-S3";
#elif defined(CONFIG_IDF_TARGET_ESP32S31)
            return "ESP32-S31";
#elif defined(CONFIG_IDF_TARGET_ESP32C2)
            return "ESP32-C2";
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
            return "ESP32-C3";
#elif defined(CONFIG_IDF_TARGET_ESP32C5)
            return "ESP32-C5";
#elif defined(CONFIG_IDF_TARGET_ESP32C6)
            return "ESP32-C6";
#elif defined(CONFIG_IDF_TARGET_ESP32C61)
            return "ESP32-C61";
#elif defined(CONFIG_IDF_TARGET_ESP32H2)
            return "ESP32-H2";
#elif defined(CONFIG_IDF_TARGET_ESP32H4)
            return "ESP32-H4";
#elif defined(CONFIG_IDF_TARGET_ESP32H21)
            return "ESP32-H21";
#elif defined(CONFIG_IDF_TARGET_ESP32P4)
            return "ESP32-P4";
#else
            return "ESP32 (Generic/Unknown Target)";
#endif
            // Note: no CONFIG_IDF_TARGET_ESP32E22 branch — confirmed via
            // the harvester that ESP-IDF has no SoC target for E22 at
            // all (RCP architecture). See harvester/soc_caps.harvested.yaml.
        }

#if ESP32STATE_HAS_IDF_VERSION
        /**
         * @brief Agnostic read-only getter for system reset cause.
         */
        static esp_reset_reason_t getResetReason() {
            return esp_reset_reason();
        }

        /**
         * @brief Agnostic read-only getter for sleep wake-up cause.
         */
        static esp_sleep_source_t getWakeupCause() {
            return esp_sleep_get_wakeup_cause();
        }
#endif
    };

    // ------------------------------------------------------------------
    // Core engine
    // ------------------------------------------------------------------

    /**
     * @brief Constructor with an optional list of conditions and a default callback.
     */
    ESP32State(const std::vector<ConditionPair>& conditions = {}, Callback defaultCallback = nullptr);

    /**
     * @brief Evaluates all conditions and runs matched callbacks.
     */
    AnalysisResult analyze();

    /**
     * @brief Dynamically adds a condition with its callback.
     */
    void addCondition(const Condition& condition, const Callback& callback);

private:
    std::vector<ConditionPair> conditions_;
    Callback defaultCallback_;
};

// Compile-time flag: strips all logging code when built with -DESP32STATE_DISABLE_LOGGING
#if defined(ESP32STATE_DISABLE_LOGGING)
    #define ESP32STATE_LOG(level, format, ...) ((void)0)
#else
    #define ESP32STATE_LOG(level, format, ...) ESP32State::log(level, format, ##__VA_ARGS__)
#endif

#endif // ESP32STATE_H
