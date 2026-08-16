/**
 * @file StartupConditions.h
 * @brief Persistent error handling helper functions and conditions for ESP32State.
 */

#ifndef STARTUPCONDITIONS_H
#define STARTUPCONDITIONS_H

#include <Arduino.h>
#include <ESP32State.h>
#include <Preferences.h>
#include <esp_system.h>
#include <vector>

// Inline declaration prevents linker errors across multiple translation units
inline uint32_t panicResetCounter = 0;
inline uint32_t powerOnResetCounter = 0;
inline uint32_t unknownResetCounter = 0;

inline Preferences preferences;

/**
 * @brief Loads error counters from Non-Volatile Storage (NVS).
 * @return true if loaded successfully, false otherwise.
 */
inline bool loadCountersFromNVS() {
    if (!preferences.begin("error_counters", true)) {
        return false;
    }

    panicResetCounter = preferences.getUInt("panicCounter", 0);
    powerOnResetCounter = preferences.getUInt("powerOnCounter", 0);
    unknownResetCounter = preferences.getUInt("unknownCounter", 0);

    preferences.end();
    return true;
}

/**
 * @brief Saves error counters to Non-Volatile Storage (NVS).
 * @return true if saved successfully, false otherwise.
 */
inline bool saveCountersToNVS() {
    if (!preferences.begin("error_counters", false)) {
        return false;
    }

    preferences.putUInt("panicCounter", panicResetCounter);
    preferences.putUInt("powerOnCounter", powerOnResetCounter);
    preferences.putUInt("unknownCounter", unknownResetCounter);

    preferences.end();
    return true;
}

/**
 * @brief Resets all error counters to zero and persists them in NVS.
 */
inline void resetAllCounters() {
    panicResetCounter = 0;
    powerOnResetCounter = 0;
    unknownResetCounter = 0;
    saveCountersToNVS();
}

/**
 * @brief Increments the counter for unknown resets and saves it to NVS.
 */
inline void incrementUnknownResetCounter() {
    unknownResetCounter++;
    saveCountersToNVS();
}

/**
 * @brief Retrieves startup conditions for analyzing reset events.
 * @return std::vector<ESP32State::ConditionPair>
 */
inline std::vector<ESP32State::ConditionPair> getStartupConditions() {
    return {
        // Condition 1: Panic reset
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() {
                panicResetCounter++;
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected! Counter: %lu", panicResetCounter);
                saveCountersToNVS();
            }
        },

        // Condition 2: Power-on reset
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() {
                powerOnResetCounter++;
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset detected! Counter: %lu", powerOnResetCounter);
                saveCountersToNVS();
            }
        }
    };
}

#endif // STARTUPCONDITIONS_H
