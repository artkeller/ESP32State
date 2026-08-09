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

    // Log-Level & Ausgabestream konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - AdvancedConditions");

    // Instanz mit vordefinierten statischen Bedingungen aus der Header-Datei erstellen
    ESP32State analyzer(getRestartConditions());

    /**
     * Dynamische Bedingung 1: Deep-Sleep Reset
     */
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Deep-sleep reset detected."); }
    );

    /**
     * Dynamische Bedingung 2: Software Reset (esp_restart)
     */
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_SW; },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Software reset detected via esp_restart()."); }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Analyse durchführen
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Ready! Tested: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Einmalige Analyse in setup()
}
