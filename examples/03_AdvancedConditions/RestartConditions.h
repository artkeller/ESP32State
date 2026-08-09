/**
 * @file RestartConditions.h
 * @brief Definition of startup conditions and corresponding callbacks for ESP32State.
 */

#ifndef RESTARTCONDITIONS_H
#define RESTARTCONDITIONS_H

#include <ESP32State.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <vector>

/**
 * @brief Retrieves a list of restart conditions and their corresponding callback functions.
 */
inline std::vector<ESP32State::ConditionPair> getRestartConditions() {
    return {
        // Condition 1: Panic reset
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected."); }
        },
        
        // Condition 2: Unknown reset reason
        {
            []() { return esp_reset_reason() == ESP_RST_UNKNOWN; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Reset reason could not be determined."); }
        },
        
        // Condition 3: Power-on reset
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on detected."); }
        },
        
        // Condition 4: External Signal Wakeup (EXT0)
        {
            []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup by external signal detected."); }
        }
    };
}

#endif // RESTARTCONDITIONS_H
