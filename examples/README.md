# ESP32State – Beispiele & Anwendungsfälle

Diese Beispielsammlung demonstriert die Funktionsweise der **ESP32State**-Bibliothek für den ESP32 unter Arduino IDE und PlatformIO.

---

## 🚀 Übersicht der Beispiele

### Basic & Core Features
* **[01_BasicStartupAnalysis](./01_BasicStartupAnalysis/)**: Minimale Einbindung. Zeigt, wie Instanziierung und Auswertung direkter Reset-Bedingungen in `setup()` funktionieren.
* **[02_ConditionsInFile](./02_ConditionsInFile/)**: Auslagerung von `ConditionPair`-Vektoren in eigene Header-Dateien zur besseren Code-Strukturierung.
* **[03_DynamicConditions](./03_DynamicConditions/)**: Flexibles Hinzufügen neuer Bedingungen zur Laufzeit mittels `analyzer.addCondition()`.
* **[04_CustomLogger](./04_CustomLogger/)**: Konfiguration des internen Loggers (`LogLevel::VERBOSE`, `LogLevel::ERROR`) und Umleitung auf alternative Serielle Schnittstellen.
* **[05_AnalysisResultEvaluation](./05_AnalysisResultEvaluation/)**: Auswertung der Struktur `ESP32State::AnalysisResult` (Anzahl ausgewerteter Regeln, getroffene Regeln, Ausführungszeit).

### Advanced State & System Tracking
* **[06_PersistentErrorHandling](./06_PersistentErrorHandling/)**: Speichern und Auswerten von Fehlerzählern im NVS (Non-Volatile Storage) über Reboots hinweg.
* **[07_DeepSleepRTCTracking](./07_DeepSleepRTCTracking/)**: Verwenden von `RTC_DATA_ATTR` im RTC-SRAM zur Überwachung von Deep-Sleep-Zyklen und Schwellenwert-Aktionen.
* **[08_EnvironmentConditions](./08_EnvironmentConditions/)**: Überwachung von Hardware-Parameter-Schwellenwerten (Spannung, interne Temperatur, Brownout-Verlauf).
* **[09_SafetyAndSecurity](./09_SafetyAndSecurity/)**: Überprüfung kritischer Systemfehler wie Task-Watchdog (TWDT), Interrupt-Watchdog (IWDT) und Hardware-Exceptions.
* **[10_ComprehensiveConditions](./10_ComprehensiveConditions/)**: Vollständiges Nachschlagewerk aller ESP-IDF Reset- und Sleep-Wakeup-Gründe unter Verwendung von zwei parallelen `ESP32State`-Instanzen.

---

## 🛠️ Verwendung in der Arduino IDE

1. Öffne die Arduino IDE.
2. Navigiere zu **Datei ➔ Beispiele ➔ ESP32State**.
3. Wähle das gewünschte Beispiel aus (z.B. `01_BasicStartupAnalysis`).
4. Wähle als Target dein ESP32-Board aus (ESP32, ESP32-S2, ESP32-S3, ESP32-C3 etc.).
5. Öffne den **Seriellen Monitor** mit einer Baudrate von `115200`.

---

## ⚙️ Hardware & Chip-Kompatibilität

* Die Beispiele verwenden die nativen ESP-IDF API-Funktionen (`esp_reset_reason()`, `esp_sleep_get_wakeup_cause()`).
* **Erweiterte Reset-Gründe** (z.B. `ESP_RST_USB`, `ESP_RST_PWR_GLITCH`) werden automatisch via Präprozessor-Guards (`#ifndef CONFIG_IDF_TARGET_ESP32`) auf älteren ESP32-Chips ausgeblendet, um Kompilierungsfehler zu vermeiden.

---

## 📋 Schnellstart (Code-Snippet)

```cpp
#include <Arduino.h>
#include <ESP32State.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Logger konfigurieren
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    // Analyzer mit Inline-Bedingung erstellen
    ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "System via Power-On gestartet."); }
        }
    });

    // Analyse durchführen
    ESP32State::AnalysisResult result = analyzer.analyze();
}

void loop() {}
