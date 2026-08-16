# ESP32State – Examples & Use Cases

This example collection demonstrates how the **ESP32State** library works on ESP32 under both the Arduino IDE and PlatformIO.

---

## Overview of the examples

> **Note:** The example names below do not currently match the actual folder names under `examples/` (e.g. this section says `01_BasicStartupAnalysis`, the actual folder is `01_BasicUsage`). This table describes the intended example set; the folder names need to be reconciled with it.

### Basic & Core Features
* **[01_BasicStartupAnalysis](./01_BasicStartupAnalysis/)**: Minimal integration. Shows how instantiation and evaluation of direct reset conditions work in `setup()`.
* **[02_ConditionsInFile](./02_ConditionsInFile/)**: Moving `ConditionPair` vectors into their own header files for better code structure.
* **[03_DynamicConditions](./03_DynamicConditions/)**: Flexibly adding new conditions at runtime via `analyzer.addCondition()`.
* **[04_CustomLogger](./04_CustomLogger/)**: Configuring the internal logger (`LogLevel::VERBOSE`, `LogLevel::ERROR`) and redirecting to alternative serial interfaces.
* **[05_AnalysisResultEvaluation](./05_AnalysisResultEvaluation/)**: Evaluating the `ESP32State::AnalysisResult` structure (number of rules evaluated, rules matched, execution time).

### Advanced State & System Tracking
* **[06_PersistentErrorHandling](./06_PersistentErrorHandling/)**: Storing and evaluating error counters in NVS (Non-Volatile Storage) across reboots.
* **[07_DeepSleepRTCTracking](./07_DeepSleepRTCTracking/)**: Using `RTC_DATA_ATTR` in RTC SRAM to track deep-sleep cycles and threshold-based actions.
* **[08_EnvironmentConditions](./08_EnvironmentConditions/)**: Monitoring hardware parameter thresholds (voltage, internal temperature, brownout history).
* **[09_SafetyAndSecurity](./09_SafetyAndSecurity/)**: Checking critical system faults such as task watchdog (TWDT), interrupt watchdog (IWDT), and hardware exceptions.
* **[10_ComprehensiveConditions](./10_ComprehensiveConditions/)**: A complete reference of all ESP-IDF reset and sleep-wakeup causes, using two parallel `ESP32State` instances.

---

## Using it in the Arduino IDE

1. Open the Arduino IDE.
2. Go to **File → Examples → ESP32State**.
3. Select the example you want (e.g. `01_BasicStartupAnalysis`).
4. Select your ESP32 board as the target (ESP32, ESP32-S2, ESP32-S3, ESP32-C3, etc.).
5. Open the **Serial Monitor** at a baud rate of `115200`.

---

## Hardware & chip compatibility

* The examples use the native ESP-IDF API functions (`esp_reset_reason()`, `esp_sleep_get_wakeup_cause()`).
* **Extended reset reasons** (e.g. `ESP_RST_USB`, `ESP_RST_PWR_GLITCH`) are automatically hidden on older ESP32 chips via preprocessor guards (`#ifndef CONFIG_IDF_TARGET_ESP32`) to avoid compile errors.

---

## Quick start (code snippet)

```cpp
#include <Arduino.h>
#include <ESP32State.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Configure the logger
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    // Create an analyzer with an inline condition
    ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "System started via power-on."); }
        }
    });

    // Run the analysis
    ESP32State::AnalysisResult result = analyzer.analyze();
}

void loop() {}
```
