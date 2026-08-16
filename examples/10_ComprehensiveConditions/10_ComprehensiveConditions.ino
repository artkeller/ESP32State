/**
 * @file 10_ComprehensiveConditions.ino
 * @brief Demonstrates the use of ESP32State to analyze all ESP32 reset and wakeup causes.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "ESP32State_Conditions.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // CDC-USB Support (ESP32-C3/S3)

    // Logging konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "=== ESP Reset and Wakeup Cause Analysis with ESP32State ===");

    // Create two independent ESP32State instances
    ESP32State resetAnalyzer(getResetConditions());
    ESP32State wakeupAnalyzer(getWakeupConditions());

    // Reset-Ursachen analysieren
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "--- Analyzing Reset Causes ---");
    ESP32State::AnalysisResult resetResult = resetAnalyzer.analyze();
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Reset Evaluation Complete! Evaluated: %zu, Matched: %zu", 
                  resetResult.total_evaluated, resetResult.matched);

    // Wakeup-Ursachen analysieren
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "--- Analyzing Wakeup Causes ---");
    ESP32State::AnalysisResult wakeupResult = wakeupAnalyzer.analyze();
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup Evaluation Complete! Evaluated: %zu, Matched: %zu", 
                  wakeupResult.total_evaluated, wakeupResult.matched);
}

void loop() {
    delay(5000);
}
