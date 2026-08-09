/**
 * @file 02_ModularExample.ino
 * @brief Example showcasing how to use ESP32State in a modular way with restart conditions defined externally.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include "RestartConditions.h" // Modulare Conditions aus separater Datei

void setup() {
    // Serielle Schnittstelle initialisieren
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // Warten auf Serial (wichtig für ESP32-C3/S3 Native USB)

    // Optional: Log-Stream auf Serial konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - Modular Example");

    // Analyzer instanziieren und Bedingungen laden
    ESP32State analyzer(getRestartConditions());

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Analyse ausführen
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Complete! Tested: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Einmalige Analyse in setup()
}
