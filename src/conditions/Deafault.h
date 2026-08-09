/**
 * @file Default.h
 * @brief Standard Reset- & Wakeup-Checks (SOC_* geguardet)
 */

#ifndef ESP32STATE_CONDITIONS_H
#define ESP32STATE_CONDITIONS_H

#include "ESP32State.h"
#include <esp_system.h>
#include <esp_sleep.h>
#include "soc/soc_caps.h"

namespace ESP32StateDefaults {

inline std::vector<ESP32State::ConditionPair> getResetConditions() {
    std::vector<ESP32State::ConditionPair> conditions;

    // Universelle Reset-Gründe
    conditions.push_back({
        []() { return esp_reset_reason() == ESP_RST_POWERON; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset."); }
    });

    conditions.push_back({
        []() { return esp_reset_reason() == ESP_RST_PANIC; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "CRITICAL: Panic reset!"); }
    });

    conditions.push_back({
        []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "WARNING: Brownout reset!"); }
    });

    conditions.push_back({
        []() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Deep sleep wake reset."); }
    });

    // Hardware-Geguardet (Wird bei Modellen ohne entsprechendes Feature nicht kompiliert)
#if defined(ESP_RST_USB)
    conditions.push_back({
        []() { return esp_reset_reason() == ESP_RST_USB; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "USB peripheral reset."); }
    });
#endif

    return conditions;
}

inline std::vector<ESP32State::ConditionPair> getWakeupConditions() {
    std::vector<ESP32State::ConditionPair> conditions;

    conditions.push_back({
        []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by timer."); }
    });

#if SOC_TOUCH_SUPPORTED
    conditions.push_back({
        []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by touchpad."); }
    });
#endif

#if SOC_PM_SUPPORT_EXT0_WAKEUP
    conditions.push_back({
        []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; },
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
