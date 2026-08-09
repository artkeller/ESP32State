/**
 * @file ESP32State_Conditions.h
 * @brief Flexible default conditions for ESP32 reset and wakeup causes.
 * @author Thomas Walloschke <artkeller@gmc.de>
 */

#ifndef ESP32STATE_CONDITIONS_H
#define ESP32STATE_CONDITIONS_H

#include <ESP32State.h>
#include "ESP32StateConfig.h"
#include <esp_system.h>
#include <esp_sleep.h>
#include "soc/soc_caps.h"

namespace ESP32StateDefaults {

inline std::vector<ESP32State::Condition> getResetConditions() {
    return {
        {[]() { return esp_reset_reason() == ESP_RST_UNKNOWN; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Unknown reset detected."); }},
        
        {[]() { return esp_reset_reason() == ESP_RST_POWERON; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Power-on reset detected."); }},

        {[]() { return esp_reset_reason() == ESP_RST_PANIC; }, 
         []() { ESP32State::log(ESP32State::LogLevel::ERROR, "CRITICAL: Panic reset detected!"); }},
        
        {[]() { return esp_reset_reason() == ESP_RST_BROWNOUT; }, 
         []() { ESP32State::log(ESP32State::LogLevel::ERROR, "WARNING: Brownout reset detected!"); }},
        
        {[]() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Deep sleep wake-up reset detected."); }}
    };
}

inline std::vector<ESP32State::Condition> getWakeupConditions() {
    return {
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Wakeup caused by timer."); }},

#if SOC_TOUCH_SUPPORTED
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Wakeup caused by touchpad interrupt."); }},
#endif

#if SOC_PM_SUPPORT_EXT0_WAKEUP
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; }, 
         []() { ESP32State::log(ESP32State::LogLevel::INFO, "Wakeup caused by external signal (EXT0)."); }},
#endif
    };
}

inline void runDefaultDiagnostics() {
    auto resets = getResetConditions();
    auto wakeups = getWakeupConditions();
    resets.insert(resets.end(), wakeups.begin(), wakeups.end());

    ESP32State manager(resets);
    manager.analyze();
}

} // namespace ESP32StateDefaults

#endif // ESP32STATE_CONDITIONS_H
