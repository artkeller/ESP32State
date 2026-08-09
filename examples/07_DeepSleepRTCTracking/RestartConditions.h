/**
 * @file RestartConditions.h
 * @brief Definition of RTC-based deep sleep tracking conditions for ESP32State.
 */

#ifndef RESTARTCONDITIONS_H
#define RESTARTCONDITIONS_H

#include <Arduino.h>
#include <ESP32State.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <vector>

// Im RTC-SRAM gespeicherte Variable übersteht Deep Sleep
RTC_DATA_ATTR inline int wakeupCounter = 0;
inline constexpr uint32_t MAX_WAKEUP_COUNT = 3;

/**
 * @brief Retrieves a list of startup conditions and their corresponding callback functions.
 */
inline std::vector<ESP32State::ConditionPair> getStartupConditions() {
    return {
        // Condition 1: Panic Reset
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected."); }
        },
        
        // Condition 2: Unknown Reset Reason
        {
            []() { return esp_reset_reason() == ESP_RST_UNKNOWN; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Reset reason could not be determined."); }
        },

        // Condition 3: Software Reset via esp_restart()
        {
            []() { return esp_reset_reason() == ESP_RST_SW; },
            []() { 
                wakeupCounter = 1;
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Reset triggered by esp_restart(). Current RTC Wakeup Counter: %d", wakeupCounter); 
            }
        },
         
        // Condition 4: Power-On Reset
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { 
                wakeupCounter = 1;
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on detected. Current RTC Wakeup Counter: %d", wakeupCounter); 
            }
        },
         
        // Condition 5: Wakeup by Timer
        {
            []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER; },
            []() { 
                wakeupCounter++;
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup by timer detected. Current RTC Wakeup Counter: %d", wakeupCounter); 
                if (wakeupCounter >= MAX_WAKEUP_COUNT) {
                    wakeupCounter = 0; // Zurücksetzen nach Maximum
                    ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Maximum number of wakeups reached (%u). Resetting counter.", MAX_WAKEUP_COUNT);
                }
            }
        }
    };
}

#endif // RESTARTCONDITIONS_H
