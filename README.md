# ESP32State
ESP32 System State &amp; Integrity Manager


---

# ESP32State: Source Documentation

## Overview
`ESP32State` is a lightweight, zero-overhead utility designed to evaluate ESP32 startup conditions, deep-sleep wake causes, and post-mortem reset reasons. It supports flexible output stream routing, conditional logging levels, and complete compile-time dead-code elimination for production builds.

---

## Key Features

- **Stream Routing & `NULL` Safety**: Route output to `Serial`, `Serial1`, SD files, custom `Print` implementations, or pass `nullptr` to suppress output safely.
- **Zero-Overhead Release Builds**: Build with `-DESP32STATE_DISABLE_LOGGING` to strip all log formatting and string constants from Flash (**0 Bytes overhead**).
- **Hardware-Aware Guards**: Features unsupported by the target SOC (e.g., Touchpad on ESP32-C3) are stripped at compile time via ESP-IDF `SOC_*` capabilities.

---

## Configuration & Usage

### 1. Basic Setup & Custom Output Routing

```cpp
#include <Arduino.h>
#include <ESP32State.h>
#include <ESP32State_Conditions.h>

void setup() {
    Serial.begin(115200);

    // Route logs to Serial with INFO level
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    // Execute predefined default diagnostics
    ESP32StateDefaults::runDefaultDiagnostics();
}

void loop() {}
