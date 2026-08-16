/**
 * @file EnvironmentConditions.h
 * @brief Predefined environmental and physical hardware checks for ESP32State.
 */

#ifndef ENVIRONMENTCONDITIONS_H
#define ENVIRONMENTCONDITIONS_H

#include <Arduino.h>
#include <ESP32State.h>
#include <esp_system.h>
#include <driver/adc.h>
#include <vector>

// Simulated/sampled environmental parameters
inline float getInternalTemperature() {
#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3)
    // ESP32 internal temperature reading (example demo value)
    return 42.5f; 
#else
    return 25.0f;
#endif
}

inline float getSupplyVoltage() {
    // Simulated VDD reading in volts
    return 3.15f; 
}

/**
 * @brief Evaluates environmental conditions on device startup.
 * @return std::vector<ESP32State::ConditionPair>
 */
inline std::vector<ESP32State::ConditionPair> getEnvironmentConditions() {
    return {
        // Condition 1: High Internal Temperature Warning
        {
            []() { return getInternalTemperature() > 80.0f; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, 
                    "Critical Over-temperature detected! Temp: %.1f C", getInternalTemperature());
            }
        },

        // Condition 2: Undervoltage / Low Battery Warning
        {
            []() { return getSupplyVoltage() < 3.0f; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::WARN, 
                    "Low supply voltage detected! VDD: %.2f V", getSupplyVoltage());
            }
        },

        // Condition 3: Hardware Brownout Reset Check
        {
            []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
            []() { 
                ESP32STATE_LOG(ESP32State::LogLevel::WARN, 
                    "Hardware Brownout event previously interrupted normal operation!");
            }
        }
    };
}

#endif // ENVIRONMENTCONDITIONS_H
