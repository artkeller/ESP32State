/**
 * @file 02_ModularExample.ino
 * @brief Example showcasing how to use ESP32State in a modular way with restart conditions defined externally.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "RestartConditions.h" // Modular conditions from a separate file

void setup() {
    // Initialize the serial interface
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // Wait for Serial (important for ESP32-C3/S3 native USB)

    // Optional: configure the log stream to Serial
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - Modular Example");

    // Instantiate the analyzer and load conditions
    ESP32State analyzer(getRestartConditions());

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Run the analysis
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Complete! Tested: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Runs a single analysis inside setup()
}
