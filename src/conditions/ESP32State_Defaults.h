/**
 * @file ESP32State_Defaults.h
 * @brief Standard reset & wakeup checks, guarded by the generated
 *        ESP32STATE_HAS_* capability macros (see
 *        variants/ESP32State_Capabilities.h) instead of raw SOC_* /
 *        ESP_RST_* identifiers.
 *
 * Two things changed from the original version of this file:
 *
 * 1. The old `#if defined(ESP_RST_USB)` guard never actually worked:
 *    ESP_RST_USB is a plain C++ enumerator, not a preprocessor macro,
 *    and #ifdef/#if defined() cannot see enum members at all — the
 *    condition silently, permanently evaluated to false on every
 *    target and every ESP-IDF version. Replaced with the generated,
 *    version-gated ESP32STATE_HAS_RESET_USB, which is a real macro.
 * 2. `#if SOC_TOUCH_SUPPORTED` referenced a macro that does not exist
 *    anywhere in ESP-IDF (verified against real soc_caps.h across all
 *    13 targets that have one — see harvester/soc_caps.harvested.yaml).
 *    Replaced with the generated ESP32STATE_HAS_TOUCH_SENSOR, built
 *    from the real macro, SOC_TOUCH_SENSOR_SUPPORTED.
 */

#ifndef ESP32STATE_CONDITIONS_H
#define ESP32STATE_CONDITIONS_H

#include "ESP32State.h"

namespace ESP32StateDefaults {

inline std::vector<ESP32State::ConditionPair> getResetConditions() {
    std::vector<ESP32State::ConditionPair> conditions;

    // Universal reset reasons — present since the oldest ESP-IDF
    // version this project harvests against (v5.0), on every target.
    conditions.push_back({
        []() { return ESP32State::HAL::getResetReason() == ESP_RST_POWERON; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset."); }
    });

    conditions.push_back({
        []() { return ESP32State::HAL::getResetReason() == ESP_RST_PANIC; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "CRITICAL: Panic reset!"); }
    });

    conditions.push_back({
        []() { return ESP32State::HAL::getResetReason() == ESP_RST_BROWNOUT; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "WARNING: Brownout reset!"); }
    });

    conditions.push_back({
        []() { return ESP32State::HAL::getResetReason() == ESP_RST_DEEPSLEEP; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Deep sleep wake reset."); }
    });

    // Version-guarded (ESP32STATE_HAS_RESET_USB, generated from a real
    // ESP-IDF version harvest — first appeared in v5.2 — instead of the
    // non-functional "#if defined(ESP_RST_USB)" this file used to have).
#if ESP32STATE_HAS_RESET_USB
    conditions.push_back({
        []() { return ESP32State::HAL::getResetReason() == ESP_RST_USB; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "USB peripheral reset."); }
    });
#endif

    return conditions;
}

inline std::vector<ESP32State::ConditionPair> getWakeupConditions() {
    std::vector<ESP32State::ConditionPair> conditions;

    conditions.push_back({
        []() { return ESP32State::HAL::getWakeupCause() == ESP_SLEEP_WAKEUP_TIMER; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by timer."); }
    });

    // Target-guarded (ESP32STATE_HAS_TOUCH_SENSOR, generated from real
    // soc_caps.h data — instead of the fictional "SOC_TOUCH_SUPPORTED").
#if ESP32STATE_HAS_TOUCH_SENSOR
    conditions.push_back({
        []() { return ESP32State::HAL::getWakeupCause() == ESP_SLEEP_WAKEUP_TOUCHPAD; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by touchpad."); }
    });
#endif

    // Target-guarded (ESP32STATE_HAS_PM_SUPPORT_EXT0_WAKEUP — generated
    // from the real SOC_PM_SUPPORT_EXT0_WAKEUP macro alone, not OR'd
    // with SOC_RTCIO_HOLD_SUPPORTED like the previous HAL did. That OR
    // made this condition compile in on chips that do not actually
    // support EXT0 wakeup, e.g. C6/H2/H4/P4/H21/S31 — confirmed against
    // the harvested capability matrix).
#if ESP32STATE_HAS_PM_SUPPORT_EXT0_WAKEUP
    conditions.push_back({
        []() { return ESP32State::HAL::getWakeupCause() == ESP_SLEEP_WAKEUP_EXT0; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by EXT0 signal."); }
    });
#endif

    return conditions;
}

inline ESP32State::AnalysisResult runDefaultDiagnostics() {
    auto resets = getResetConditions();
    auto wakeups = getWakeupConditions();
    resets.insert(resets.end(), wakeups.begin(), wakeups.end());

    ESP32State manager(resets);
    return manager.analyze();
}

} // namespace ESP32StateDefaults

#endif // ESP32STATE_CONDITIONS_H
