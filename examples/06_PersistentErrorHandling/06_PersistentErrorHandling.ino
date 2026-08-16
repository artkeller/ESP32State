/**
 * @file 06_PersistentErrorHandling.ino
 * @brief Example sketch for tracking and handling reset events on ESP32 with persistent NVS storage.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "StartupConditions.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // CDC-USB Support (ESP32-C3/S3)

    // Configure logging
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - PersistentErrorHandling");

    // Load counters from NVS
    if (!loadCountersFromNVS()) {
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Error loading counters! Initializing all counters to zero.");
        resetAllCounters();
    }

    // Instantiate the analyzer and load the predefined conditions
    ESP32State analyzer(getStartupConditions());

    // Dynamic condition: reset counters on a brownout event
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
        []() {
            resetAllCounters();
            ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Brownout detected! All counters reset.");
        }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzing reset conditions...");
    
    // Run the analysis
    ESP32State::AnalysisResult result = analyzer.analyze();

    // Fallback logic: in case no specific condition was triggered
    if (result.matched == 0) {
        incrementUnknownResetCounter();
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Unknown reset reason! Counter: %u", unknownResetCounter);
    }

    // Persist the updated counter values to NVS
    if (!saveCountersToNVS()) {
        ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Error saving counters to NVS!");
    }

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Ready!");
}

void loop() {
    // Runs a single analysis inside setup()
}
