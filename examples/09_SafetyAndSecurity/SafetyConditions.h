/**
 * @file SafetyConditions.h
 * @brief Safety, watchdog, and integrity condition checks for ESP32State.
 */

#ifndef SAFETYCONDITIONS_H
#define SAFETYCONDITIONS_H

#include <Arduino.h>
#include <ESP32State.h>
#include <esp_system.h>
#include <vector>

/**
 * @brief Evaluates system integrity and crash-related reset reasons.
 * @return std::vector<ESP32State::ConditionPair>
 */
inline std::vector<ESP32State::ConditionPair> getSafetyConditions() {
    return {
        // Condition 1: Task Watchdog Reset
        {
            []() { return esp_reset_reason() == ESP_RST_TASK_WDT; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, 
                    "SAFETY ALERT: Task Watchdog Timeout occurred! Check for stuck loops/deadlocks.");
            }
        },

        // Condition 2: Interrupt Watchdog Reset
        {
            []() { return esp_reset_reason() == ESP_RST_INT_WDT; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, 
                    "SAFETY ALERT: Interrupt Watchdog Reset! ISR ran too long or disabled interrupts.");
            }
        },

        // Condition 3: Panic / Core Dump Reset
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, 
                    "SECURITY/SAFETY ALERT: Hardware Panic Exception (Null pointer / Stack overflow).");
            }
        }
    };
}

#endif // SAFETYCONDITIONS_H
