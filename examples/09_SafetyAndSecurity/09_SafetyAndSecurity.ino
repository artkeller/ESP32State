/**
 * @file 09_SafetyAndSecurity.ino
 * @brief Demonstrates safety-critical exception tracking using ESP32State.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "SafetyConditions.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {}

    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - SafetyAndSecurity");

    ESP32State safetyAnalyzer(getSafetyConditions());

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Performing Safety & Integrity Audit...");
    ESP32State::AnalysisResult result = safetyAnalyzer.analyze();

    if (result.matched > 0) {
        ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "System previously crashed due to a safety violation!");
        // A safe mode or emergency recovery could be triggered here
    } else {
        ESP32STATE_LOG(ESP32State::LogLevel::INFO, "System booted cleanly without safety violations.");
    }
}

void loop() {
    delay(5000);
}
