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

    // Logging konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - PersistentErrorHandling");

    // Zähler aus NVS laden
    if (!loadCountersFromNVS()) {
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Error loading counters! Initializing all counters to zero.");
        resetAllCounters();
    }

    // Analyzer instanziieren und prädefinierte Bedingungen laden
    ESP32State analyzer(getStartupConditions());

    // Dynamische Bedingung: Counter bei Brownout-Event zurücksetzen
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
        []() {
            resetAllCounters();
            ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Brownout detected! All counters reset.");
        }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzing reset conditions...");
    
    // Analyse durchführen
    ESP32State::AnalysisResult result = analyzer.analyze();

    // Fallback-Logik: Falls keine spezifische Bedingung getriggert wurde
    if (result.matched == 0) {
        incrementUnknownResetCounter();
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Unknown reset reason! Counter: %u", unknownResetCounter);
    }

    // Aktualisierte Zählerwerte in NVS speichern
    if (!saveCountersToNVS()) {
        ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Error saving counters to NVS!");
    }

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Ready!");
}

void loop() {
    // Einmalige Analyse im setup()
}
