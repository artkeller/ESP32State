/**
 * @file 01_BasicUsage.ino
 * @brief Basic example demonstrating how to use ESP32State to evaluate reset causes.
 */

#include <Arduino.h>
#include <ESP32State.h>

/**
 * @brief Defines the condition-callback pairs to be evaluated by ESP32State.
 * @return std::vector<ESP32State::ConditionPair>
 */
std::vector<ESP32State::ConditionPair> getMyStateConditions() {
    return {
        // Condition 1: Unknown reset reason (e.g. after fresh flash)
        {
            []() { return esp_reset_reason() == ESP_RST_UNKNOWN; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Memory reflashed or unknown reset reason."); }
        },

        // Condition 2: Regular Power-On Reset
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Cold Power-On reset detected."); }
        },

        // Condition 3: Panic / Crash Reset (Emergency Trap)
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() {
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected! Halting execution...");
                while (true) {
                    delay(100);
                }
            }
        }
    };
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // CDC-USB Support (C3/S3)

    // Optional: Logging-Instanz auf Serial setzen
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - BasicUsage");

    // Instanziierung mit Condition-Vector und optionalem "Completion Callback"
    ESP32State analyzer(
        getMyStateConditions(),
        []() {
            ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analysis complete. No critical faults triggered.");
        }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer finished. Evaluated: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Einmalige Ausführung in setup()
}
