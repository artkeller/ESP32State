/**
 * @file 07_DeepSleepRTCTracking.ino
 * @brief Example demonstrating RTC-based Deep Sleep tracking with ESP32State.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "RestartConditions.h"

// Sleep duration: 10 seconds in microseconds
constexpr uint64_t SLEEP_DURATION_US = 10ULL * 1000000ULL;

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // CDC-USB Support (ESP32-C3/S3)

    // Logging configuration
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - DeepSleepRTCTracking");

    // Instantiate the analyzer with the conditions from RestartConditions.h
    ESP32State analyzer(getStartupConditions());

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Start Analyzer...");
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Complete! Evaluated: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);

    // Flow control based on the RTC wakeup counter
    if (wakeupCounter == 0) {
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "-> Threshold reached or first boot. Performing software restart (esp_restart)...");
        delay(1000);
        esp_restart();
    } else {
        ESP32STATE_LOG(ESP32State::LogLevel::INFO, "-> Going to deep sleep for %llu seconds...", SLEEP_DURATION_US / 1000000ULL);
        esp_sleep_enable_timer_wakeup(SLEEP_DURATION_US);
        esp_deep_sleep_start();
    }
}

void loop() {
    // Never reached during deep sleep
}
