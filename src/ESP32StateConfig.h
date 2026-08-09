/**
 * @file ESP32StateConfig.h
 * @brief Configurable output stream routing and logging for ESP32State.
 */

#ifndef ESP32STATE_CONFIG_H
#define ESP32STATE_CONFIG_H

#include <Arduino.h>

// Compile-Time Flag: Löscht bei -DESP32STATE_DISABLE_LOGGING sämtlichen Log-Code
#if defined(ESP32STATE_DISABLE_LOGGING)
    #define ESP32STATE_LOG(level, format, ...) ((void)0)
#else
    #define ESP32STATE_LOG(level, format, ...) ESP32State::log(level, format, ##__VA_ARGS__)
#endif

namespace ESP32State {

enum class LogLevel {
    NONE = 0,   ///< Keinerlei Ausgaben
    ERROR = 1,  ///< Nur Fehler (Panic, Brownout etc.)
    INFO = 2,   ///< Normale Diagnosedaten
    VERBOSE = 3 ///< Ausführliches Logging
};

struct Config {
    Print* outputStream = &Serial;      ///< Ziel-Stream (&Serial, &Serial1, &File oder nullptr)
    LogLevel logLevel = LogLevel::INFO; ///< Standard Loglevel
    bool enablePrefix = true;           ///< Prefix "[ESP32State]" aktivieren
};

// Globale Bibliotheks-Konfiguration
extern Config globalConfig;

/**
 * @brief Konfiguriert das Output-Routing zur Laufzeit.
 * @param stream Zeiger auf Print-Objekt oder nullptr für Stummschaltung.
 * @param level Gewünschtes Log-Level.
 */
inline void configure(Print* stream, LogLevel level = LogLevel::INFO) {
    globalConfig.outputStream = stream;
    globalConfig.logLevel = level;
}

/**
 * @brief Interne Print-Hilfsfunktion mit nullptr-Check und Level-Filtering.
 */
inline void log(LogLevel level, const char* format, ...) {
    if (globalConfig.outputStream == nullptr || level > globalConfig.logLevel) {
        return; // nullptr oder Level unterschritten -> Abbruch vor Formatting
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
