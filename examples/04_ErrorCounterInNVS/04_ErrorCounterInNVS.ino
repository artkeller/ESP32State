/**
 * @file 04_ErrorCounterInNVS.ino
 * @brief Example usage of ESP32State with NVS-based error counters.
 * 
 * Demonstrates tracking reset counts across reboots using Preferences (NVS).
 */

#include <Arduino.h>
#include <ESP32State.h>
#include <Preferences.h>
#include <esp_sleep.h>

// Instanz für Non-Volatile Storage (NVS)
Preferences preferences;

/**
 * @brief Defines startup conditions and their associated callbacks.
 * @return std::vector<ESP32State::ConditionPair>
 */
std::vector<ESP32State::ConditionPair> getStartupConditions() {
    return {
        // Condition 1: Increment panic reset counter
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() {
                int panicResetCounter = preferences.getInt("panicCount", 0);
                panicResetCounter++;
                preferences.putInt("panicCount", panicResetCounter);
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected! Total Panic Count: %d", panicResetCounter);
            }
        },

        // Condition 2: Increment power-on reset counter
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() {
                int powerOnResetCounter = preferences.getInt("powerOnCount", 0);
                powerOnResetCounter++;
                preferences.putInt("powerOnCount", powerOnResetCounter);
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset detected! Total Power-On Count: %d", powerOnResetCounter);
            }
        },

        // Condition 3: Report external wakeup (EXT0)
        {
            []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; },
            []() {
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup detected from external signal (EXT0).");
            }
        }
    };
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // Warten auf CDC-Serial (ESP32-C3/S3)

    // Logging konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - ErrorCounterInNVS");

    // NVS Namespace öffnen
    preferences.begin("errorAnalyzer", false);

    // Instanz mit vorbereiteten Bedingungen erstellen
    ESP32State analyzer(getStartupConditions());

    // Dynamische Bedingung: Counter bei Brownout-Event zurücksetzen
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
        []() {
            preferences.putInt("panicCount", 0);
            preferences.putInt("powerOnCount", 0);
            ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Brownout detected! All error counters reset in NVS.");
        }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Analyse durchführen
    ESP32State::AnalysisResult result = analyzer.analyze();

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analysis complete. Evaluated: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);

    // NVS-Verbindung sauber schließen
    preferences.end();
}

void loop() {
    // Einmalige Analyse im setup()
}
