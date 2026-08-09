/**
 * @file ESP32StateConfig.h
 * @brief Configuration and stream routing for ESP32State.
 */

#ifndef ESP32STATE_CONFIG_H
#define ESP32STATE_CONFIG_H

#include <Arduino.h>

namespace ESP32State {

enum class LogLevel {
    NONE = 0,   ///< Stummschaltung (Keine Log-Ausgaben)
    ERROR = 1,  ///< Nur kritische Fehler (z. B. Panic, Panic-Reset, Brownout)
    INFO = 2,   ///< Standard-Informationen (Start-Ursache, Wakeup-Events)
    VERBOSE = 3 ///< Ausführliche System-Diagnose
};

struct Config {
    Print* outputStream = &Serial;      ///< Zeiger auf Print-Stream (z. B. &Serial, &Serial1, oder nullptr)
    LogLevel logLevel = LogLevel::INFO; ///< Standard-Loglevel
    bool enablePrefix = true;           ///< Ob "[ESP32State]" vorangestellt werden soll
};

// Globale Bibliotheks-Konfiguration
extern Config globalConfig;

/**
 * @brief Passt die Ausgabe-Konfiguration der Bibliothek an.
 * @param stream Zeiger auf ein Print-Objekt (z. B. &Serial, &File) oder nullptr für Stummschaltung.
 * @param level Das gewünschte Log-Level.
 */
inline void configure(Print* stream, LogLevel level = LogLevel::INFO) {
    globalConfig.outputStream = stream;
    globalConfig.logLevel = level;
}

/**
 * @brief Interne Print-Hilfsfunktion (beachtet Stream NULL-Check und LogLevel)
 */
inline void log(LogLevel level, const char* format, ...) {
    if (globalConfig.outputStream == nullptr || level > globalConfig.logLevel) {
        return; // Stummschaltung oder Loglevel zu niedrig
    }

    if (globalConfig.enablePrefix) {
        globalConfig.outputStream->print("[ESP32State] ");
    }

    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    globalConfig.outputStream->println(buffer);
}

} // namespace ESP32State

#endif // ESP32STATE_CONFIG_H
