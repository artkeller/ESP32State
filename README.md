# ESP32State

[![CRA Status](https://img.shields.io/badge/CRA-Exempt%20(pure%20OSS)-informational)](./CRA-EXEMPTION.md)
[![License](https://img.shields.io/badge/License-MIT-blue?style=flat-square)](./LICENSE)
[![Version](https://img.shields.io/badge/Version-2026.08-brightgreen?style=flat-square)](./README.md)
[![Status](https://img.shields.io/badge/Status-In%20Progress-orange?style=flat-square)](https://github.com/artkeller/ESP32State)

![](assets/social/d5befd26-6023-4565-adab-ef1f0197d0db-2.jpg)

**Deterministic diagnostic layer & panic-room logic for ESP32**

An ESP32 in the field restarts for widely different reasons: cold boot, deep-sleep timer, brownout, watchdog reset, or a crash (panic). By default, the ESP32 blindly runs `setup()` after every restart – without the application deterministically knowing which safety state the system woke up into.

**`ESP32State`** closes this safety gap: it provides a side-effect-free check matrix for system startup. Restart and wakeup causes, as well as critical hardware conditions, are registered and evaluated **explicitly, transparently, and without unpredictable code magic**.

* **Operational-safety focus:** Helps developers demonstrate robust boot/wakeup state handling – a building block for robustness requirements such as those set out by the EU Cyber Resilience Act (CRA), without itself being a CRA conformity artifact (see [CRA-EXEMPTION.md](./CRA-EXEMPTION.md)).
* **Zero-Magic & Sealable:** No hidden cascade effects, no hidden state machines. What has been defined is executed safely and auditably.
* **Zero-Overhead:** Compact, performant, and ideal for embedded systems.

---

## Motivation for this library

An ESP32 in the field doesn't just "restart somehow" – it starts from one of several fundamentally different states, and none of them are distinguishable by default without writing your own register evaluation: cold boot, deep-sleep timer, brownout, watchdog reset, panic. ESP-IDF only provides two raw enum functions for this (`esp_reset_reason()`, `esp_sleep_get_wakeup_cause()`) – interpreting what follows from them for the application is left entirely to the developer, and in practice is almost always rewritten as a copy-pasted `switch`/`case` block per project (see the countless one-off examples in Espressif's own `arduino-esp32` documentation and across the community). A chip-agnostic, declarative library that systematizes this is not one I'm aware of, despite looking. `ESP32State` closes this gap: a side-effect-free check matrix that registers restart and wakeup causes, as well as critical hardware conditions, explicitly, transparently, and auditably.

### Why this is particularly safety-relevant with deep sleep

This topic comes up predominantly in the context of deep-sleep applications – battery-powered sensor nodes, self-contained field devices, industrial IoT endpoints – and that's no coincidence: **the operating environment can change during the sleep cycle without the firmware knowing about it.** Battery voltage may have dropped below the brownout threshold and recovered, temperature may have gone outside specified operating limits, a sensor may have physically come loose, RTC memory may have been corrupted by a voltage dip. A device that blindly resumes at `setup()` after waking, as if nothing happened, skips exactly the checks that would be needed in a changed environment before measuring, transmitting, or actuating as usual again.

**This makes robust boot-state handling a matter of operational safety, not convenience** – and this is where the actual connection to product regulation lies: the Cyber Resilience Act's essential requirements (Annex I) call for, among other things, products with digital elements to be designed to provide an appropriate level of protection regarding availability and resilience against operational disruptions. Similar robustness expectations show up in adjacent regulatory frameworks too (general product safety, sector-specific safety standards). `ESP32State` itself – as laid out in [CRA-EXEMPTION.md](./CRA-EXEMPTION.md) – is, as an unpaid open-source project by a private individual, not directly covered by the CRA and makes no conformity claim. The connection is a different one: anyone building a product that must satisfy such requirements needs robust, auditable boot-state logic as a building block anyway – `ESP32State` provides exactly that building block, without itself being a compliance artifact.

* **Normal deep sleep?** → Send readings and go straight back to sleep.
* **Power failure (power-on / brownout)?** → Recalibrate peripherals, check RTC RAM.
* **Critical crash (watchdog / panic)?** → Switch to the safe emergency mode (*panic room*), log the error to NVS, and prepare for re-flashing.

Instead of writing unreadable `if-else` nesting over Espressif's C registers, `ESP32State` wraps this evaluation into clean, declarative **ConditionPairs** (condition + action).

---

## Multi-SoC support & hardware HAL

`ESP32State` is designed to be agnostic and supports **all 14 ESP32 silicon variants**:
`ESP32` | `ESP32-S2` | `ESP32-S3` | `ESP32-S31` | `ESP32-C2` | `ESP32-C3` | `ESP32-C5` | `ESP32-C6` | `ESP32-C61` | `ESP32-H2` | `ESP32-H4` | `ESP32-H21` | `ESP32-P4` | `ESP32-E22`

The built-in Hardware Abstraction Layer (`variants/ESP32State_HAL.h`) resolves differing register layouts and wakeup capabilities (EXT0/EXT1, touch, ULP, RTC RAM) entirely at **compile time**. Application code remains 100% agnostic, with no need for macro branching in the main program.

> **Hardware reference:** A detailed overview of all chip specifications, register requirements, and deep-sleep characteristics can be found in the [**ESP32Features**](https://github.com/artkeller/ESP32Features) repository.

---



## Quick start (minimal example)

```cpp
#include <Arduino.h>
#include <ESP32State.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    // 1. Configure the logger (optional, shows details in the Serial Monitor)
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    // 2. Define conditions: IF (condition met) THEN (run action)
    ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "First boot: fresh power-on!"); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Woke up from deep sleep."); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "WARNING: Last restart was a crash!"); }
        }
    });

    // 3. Start the analysis (checks all rules and runs matching callbacks)
    ESP32State::AnalysisResult result = analyzer.analyze();
}

void loop() {
    // Stays empty – diagnostics run once at boot
}

```

---

## Structuring a professional project

To keep `main.cpp` or the `.ino` file readable, move the logic out as follows:

### 1. File `setup_state.h`

```cpp
#ifndef SETUP_STATE_H
#define SETUP_STATE_H

#include <ESP32State.h>

inline void initSystemState() {
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    static ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { Serial.println("Normal cold boot."); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_TASK_WDT; },
            []() { Serial.println("Error: Task watchdog reset!"); }
        }
    });

    analyzer.analyze();
}

#endif // SETUP_STATE_H

```

### 2. Main file `.ino`

```cpp
#include <Arduino.h>
#include "setup_state.h"

void setup() {
    Serial.begin(115200);
    
    // Simple, clean call!
    initSystemState();
}

void loop() {
    // Your actual application logic
}

```

---


## Practical tip for clean projects

> **Practical note:** In this library's example files (`.ino`), initialization is often placed directly inside the `setup()` function. This is done **purely for teaching purposes and readability**.
> For real project use, it's recommended to isolate the initialization code into its own file (e.g. `setup_state.h` or `setup_system.h`). The main `.ino` then only calls a single function; the main file stays readable and easy to maintain!

---

## Advanced: targeted HW queries with the HAL

For chip-specific emergency logic (e.g. when a feature like `EXT0` only exists on certain SoCs such as the ESP32, S2, or S3), the HAL provides clean feature flags. The compiler automatically eliminates the code on chips without support (**0 bytes of flash overhead**):

```cpp
#include <ESP32State.h>
#include <variants/ESP32State_HAL.h>

// 1. Query the target name, agnostically:
ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Running on Target: " ESP32State::HAL::getTargetName());

// 2. Safely check hardware capabilities at compile time:
#if ESP32STATE_HAS_EXT0_WAKEUP
    {
        []() { return ESP32State::HAL::getWakeupCause() == ESP_SLEEP_WAKEUP_EXT0; },
        []() { Serial.println("Woke up via EXT0 pin!"); }
    }
#endif

```

---

## The application primer: which example explains what?

For step-by-step learning, the [`examples/`](examples) folder contains 10 ready-made demos:

| No. | Topic | Goal |
| --- | --- | --- |
| **01** | `01_BasicStartupAnalysis` | **Getting started:** Understand the basics of analysis. |
| **02** | `02_ConditionsInFile` | **Structure:** Move rules out into their own files. |
| **03** | `03_DynamicConditions` | **Flexibility:** Add rules dynamically at runtime. |
| **04** | `04_CustomLogger` | **Diagnostics:** Control your own logging and log level. |
| **05** | `05_AnalysisResultEvaluation` | **Evaluation:** Use results (duration, matches) programmatically. |
| **06** | `06_PersistentErrorHandling` | **Memory:** Persist an error counter in EEPROM/NVS across reboots. |
| **07** | `07_DeepSleepRTCTracking` | **Energy:** Track deep-sleep cycles in RTC RAM. |
| **08** | `08_EnvironmentConditions` | **Hardware:** Monitor sensors, battery voltage & temperature. |
| **09** | `09_SafetyAndSecurity` | **Safety:** Specifically catch crashes (watchdog, panic). |
| **10** | `10_ComprehensiveConditions` | **Complete:** Overview of all ESP-IDF reset & wakeup causes. |

---

## Installation

### Arduino IDE

1. Download the repository as a `.zip`.
2. In the Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library...**

### PlatformIO

Add the library to your `platformio.ini`:

```ini
lib_deps =
    ESP32State

```

---

## License

Published under the MIT License. Free to use in private and commercial projects.
