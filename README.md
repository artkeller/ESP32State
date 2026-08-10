# ESP32State

**Diagnoseschicht für ESP32**

Ein ESP32 im Feld kann aus vielen Gründen neu starten: Stromausfall, Deep-Sleep-Timer, Druck auf den Reset-Knopf, Absturz durch Speichermangel (Panic) oder ein ausgelöster Watchdog. Standardmäßig führt der ESP32 nach jedem Neustart einfach wieder `setup()` aus – ohne dass das Anwedungsprogramm direkt weiß, **warum** es überhaupt gestartet wurde, obwohl ein ESP32 dafür grundsätzlich vorbereitet ist. 

![](assets/social/d5befd26-6023-4565-adab-ef1f0197d0db-2.jpg)

**`ESP32State`** löst genau dieses Aufgabe: Diese Bbliothek ermöglicht, Neustart- und Aufwachursachen (sowie eigene Systembedingungen) strukturiert und übersichtlich zu registrieren, auszuwerten und darauf mit passenden Aktionen zu reagieren. Auf diese weise wird ein ESP32 Projekt deutlich näher an die CRA Vorgaben geführt und unterstützt Factority-basierbar einen stabilen Bestrieb.

---

## Wazu diese Bibliothek?

Einsatzfall: Messung von Sensordaten im Feld durch einen ESP32:

* Was soll passieren, wenn er ganz normal aus dem **Deep Sleep** aufwacht? *(Daten senden und wieder schlafen)*
* Was soll passieren, wenn der Strom ausfiel (**Power-On Reset**)? *(Hardware neu initialisieren, Speicher prüfen)*
* Was soll passieren, wenn das System abgestürzt ist (**Watchdog / Panic Reset**)? *(Fehler im Speicher protokollieren, Notfall-Modus starten)*

Mit `ESP32State` werden diese Fallunterscheidungen in sogenannte **ConditionPairs** (Bedingung + Aktion) verpackt – übersichtlich, erweiterbar und wartbar.

---

## Praxistipp für saubere Projekte

> **Praxis-Hinweis:** In den Beispiel-Dateien (`.ino`) dieser Bibliothek steht die Initialisierung häufig direkt in der Funktion `setup()`. Das dient **ausschließlich der Didaktik und Lesbarkeit**.
> Für den raealen Projekteinsatz wird empfohlen, den Initialisieungscode in eine eigene Datei (z. B. `setup_state.h` oder `setup_system.h`) zu isolieren. In der Haupt-`.ino` wird nur noch eine Funktion aufgerufen; die Hauptdatei belibt übersichtlich und leicht wartbar!

---

## Schnellstart (Minimalbeispiel)

```cpp
#include <Arduino.h>
#include <ESP32State.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    // 1. Logger konfigurieren (optional, zeigt Details im Seriellen Monitor)
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    // 2. Bedingungen definieren: WENN (Bedingung erfüllt) DANN (Aktion ausführen)
    ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Erster Start: Frisch am Strom!"); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Aus dem Tiefschlaf erwacht."); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_PANIC; },
            []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "ACHTUNG: Letzter Neustart war ein Absturz!"); }
        }
    });

    // 3. Analyse starten (prüft alle Regeln und führt Treffer aus)
    ESP32State::AnalysisResult result = analyzer.analyze();
}

void loop() {
    // Bleibt leer – die Diagnose läuft einmalig beim Booten
}

```

---

## So strukturierst du es im Profi-Projekt

Damit deine `main.cpp` oder `.ino` übersichtlich bleibt, lagere die Logik wie folgt aus:

### 1. Datei `setup_state.h`

```cpp
#ifndef SETUP_STATE_H
#define SETUP_STATE_H

#include <ESP32State.h>

inline void initSystemState() {
    ESP32State::configure(&Serial, ESP32State::LogLevel::INFO);

    static ESP32State analyzer({
        {
            []() { return esp_reset_reason() == ESP_RST_POWERON; },
            []() { Serial.println("Normaler Kaltstart."); }
        },
        {
            []() { return esp_reset_reason() == ESP_RST_TASK_WDT; },
            []() { Serial.println("Fehler: Task Watchdog Reset!"); }
        }
    });

    analyzer.analyze();
}

#endif // SETUP_STATE_H

```

### 2. Hauptdatei `.ino`

```cpp
#include <Arduino.h>
#include "setup_state.h"

void setup() {
    Serial.begin(115200);
    
    // Einfacher, sauberer Aufruf!
    initSystemState();
}

void loop() {
    // Deine eigentliche Anwendungslogik
}

```

---

## Das Anwendungs-1x1: Welches Beispiel erklärt was?

Für Schritt-für-Schritt-Lernen enthält der Ordner [`examples/`](examples) 10 fertige Demos:

| Nr. | Thema | Ziel |
| --- | --- | --- |
| **01** | `01_BasicStartupAnalysis` | **Der Einstieg:** Grundlagen der Analyse verstehen. |
| **02** | `02_ConditionsInFile` | **Struktur:** Regeln in eigene Dateien auslagern. |
| **03** | `03_DynamicConditions` | **Flexibilität:** Regeln zur Laufzeit dynamisch hinzufügen. |
| **04** | `04_CustomLogger` | **Diagnose:** Eigenes Logging und Log-Level steuern. |
| **05** | `05_AnalysisResultEvaluation` | **Auswertung:** Ergebnisse (Dauer, Treffer) programmtechnisch nutzen. |
| **06** | `06_PersistentErrorHandling` | **Speicher:** Fehlerzähler im EEPROM/NVS über Reboots hinweg sichern. |
| **07** | `07_DeepSleepRTCTracking` | **Energy:** Deep-Sleep-Zyklen im RTC-RAM mitverfolgen. |
| **08** | `08_EnvironmentConditions` | **Hardware:** Sensoren, Akku-Spannung & Temperatur überwachen. |
| **09** | `09_SafetyAndSecurity` | **Sicherheit:** Abstürze (Watchdog, Panic) gezielt abfangen. |
| **10** | `10_ComprehensiveConditions` | **Vollständig:** Alle ESP-IDF Reset- & Wakeup-Gründe im Überblick. |

---

## Installation

### Arduino IDE

1. Lade das Repository als `.zip` herunter.
2. Navigiere in der Arduino IDE zu **Sketch ➔ Bibliothek einbinden ➔ .ZIP-Bibliothek hinzufügen...**

### PlatformIO

Füge die Bibliothek zu deiner `platformio.ini` hinzu:

```ini
lib_deps =
    ESP32State

```

---

## Lizenz

Veröffentlicht unter der MIT-Lizenz. Freie Nutzung in privaten und kommerziellen Projekten erlaubt.
