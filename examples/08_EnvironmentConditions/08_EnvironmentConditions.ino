/**
 * @file 08_EnvironmentConditions.ino
 * @brief Demonstrates using ESP32State to monitor environmental and operational conditions.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "EnvironmentConditions.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {}

    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - EnvironmentConditions");

    ESP32State envAnalyzer(getEnvironmentConditions());

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Evaluating Environmental Health...");
    ESP32State::AnalysisResult result = envAnalyzer.analyze();

    if (result.matched == 0) {
        ESP32STATE_LOG(ESP32State::LogLevel::INFO, "All environmental parameters within nominal ranges.");
    } else {
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "%zu environmental issues detected!", result.matched);
    }
}

void loop() {
    delay(5000);
}
