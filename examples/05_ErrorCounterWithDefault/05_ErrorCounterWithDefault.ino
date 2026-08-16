/**
 * @file 05_ErrorCounterWithDefault.ino
 * @brief Example using ESP32State with NVS persistent counters and a default fallback callback.
 */

#include <Arduino.h>
#include <ESP32State.h>
#include <Preferences.h>
#include <esp_sleep.h>

Preferences preferences;

static int panicResetCounter = 0;
static int powerOnResetCounter = 0;
static int unknownResetCounter = 0;

void loadErrorCounters() {
    preferences.begin("errorCounters", true);
    panicResetCounter = preferences.getInt("panic", 0);
    powerOnResetCounter = preferences.getInt("poweron", 0);
    unknownResetCounter = preferences.getInt("unknown", 0);
    preferences.end();
}

void saveErrorCounters() {
    preferences.begin("errorCounters", false);
    preferences.putInt("panic", panicResetCounter);
    preferences.putInt("poweron", powerOnResetCounter);
    preferences.putInt("unknown", unknownResetCounter);
    preferences.end();
}

std::vector<ESP32State::ConditionPair> getStartupConditions() {
    return {
        // Condition 1: Panic reset
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() {
                panicResetCounter++;
                ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected! Counter: %d", panicResetCounter);
                saveErrorCounters();
            }
        },

        // Condition 2: Power-on reset
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() {
                powerOnResetCounter++;
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset detected! Counter: %d", powerOnResetCounter);
                saveErrorCounters();
            }
        },

        // Condition 3: Wakeup via EXT0
#if SOC_PM_SUPPORT_EXT0_WAKEUP
        {
            []() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; },
            []() {
                ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup detected from external signal (EXT0).");
            }
        }
#endif
    };
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    while (!Serial) {} // USB-CDC (C3/S3)

    ESP32State::configure(&Serial, ESP32State::LogLevel::VERBOSE);

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Firmware started: ESP32State - ErrorCounterWithDefault");

    loadErrorCounters();

    // Create an instance with conditions and a fallback/completion handler
    ESP32State analyzer(getStartupConditions());

    // Dynamic condition: brownout resets all counters
    analyzer.addCondition(
        []() { return esp_reset_reason() == ESP_RST_BROWNOUT; },
        []() {
            panicResetCounter = 0;
            powerOnResetCounter = 0;
            unknownResetCounter = 0;
            saveErrorCounters();
            ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Brownout detected! All error counters reset.");
        }
    );

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting Analyzer...");
    
    // Run the analysis
    ESP32State::AnalysisResult result = analyzer.analyze();

    // Fallback logic: in case no specific condition was triggered
    if (result.matched == 0) {
        unknownResetCounter++;
        ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Unclassified or unknown reset condition! Counter: %d", unknownResetCounter);
        saveErrorCounters();
    }

    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Analyzer Complete! Evaluated: %zu, Matched: %zu", 
                  result.total_evaluated, result.matched);
}

void loop() {
    // Runs once inside setup()
}
