/**
 * @file 03_AdvancedConditions.ino
 * @brief Example demonstrating dynamic addition of conditions at runtime in setup().
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "RestartConditions.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // CDC-USB Support (ESP32-C3/S3)

    // Configure log level & output stream
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - AdvancedConditions");

    // Create an instance with the predefined static conditions from the header file
    ESP32State analyzer(getRestartConditions());

    /**
     * Dynamic condition 1: Deep-sleep reset
     */
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Deep-sleep reset detected."); }
    );

    /**
     * Dynamic condition 2: Software reset (esp_restart)
     */
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_SW; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Software reset detected via esp_restart()."); }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Run the analysis
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Ready! Tested: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Runs a single analysis inside setup()
}
