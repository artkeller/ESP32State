/**
 * @file ESP32State_Conditions.h
 * @brief Defines conditions for all ESP32 reset and wakeup causes using ESP32State.
 */

#ifndef ESP32STATE_CONDITIONS_H
#define ESP32STATE_CONDITIONS_H

#include <Arduino.h>
#include <ESP32State.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <vector>

/**
 * @brief Get predefined reset conditions for the ESP32.
 * @return A vector of ESP32State::ConditionPair objects.
 */
inline std::vector<ESP32State::ConditionPair> getResetConditions() {
    return {
        {[]() { return esp_reset_reason() == ESP_RST_UNKNOWN; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Unknown reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_POWERON; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Power-on reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_EXT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "External signal reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_SW; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Software reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_PANIC; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Panic reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_INT_WDT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Interrupt watchdog reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_TASK_WDT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Task watchdog reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_WDT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "General watchdog reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_DEEPSLEEP; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Deep sleep wake-up reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_BROWNOUT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Brownout reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_SDIO; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "SDIO reset detected."); }},
#ifndef CONFIG_IDF_TARGET_ESP32
        {[]() { return esp_reset_reason() == ESP_RST_USB; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "USB reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_JTAG; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "JTAG reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_EFUSE; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "eFuse reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_PWR_GLITCH; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::WARN, "Power glitch reset detected."); }},
        {[]() { return esp_reset_reason() == ESP_RST_CPU_LOCKUP; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "CPU lockup reset detected."); }}
#endif
    };
}

/**
 * @brief Get predefined wakeup conditions for the ESP32.
 * @return A vector of ESP32State::ConditionPair objects.
 */
inline std::vector<ESP32State::ConditionPair> getWakeupConditions() {
    return {
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "No defined wakeup cause."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by external signal via RTC_IO."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by external signal via RTC_CNTL."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by timer."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by touchpad interrupt."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_ULP; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by ULP program."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by GPIO (light sleep only)."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UART; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by UART (light sleep only)."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_WIFI; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by WiFi (light sleep only)."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_COCPU; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by co-processor interrupt."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Wakeup caused by co-processor crash."); }},
        {[]() { return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_BT; }, 
         []() { ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Wakeup caused by Bluetooth (light sleep only)."); }}
    };
}

#endif // ESP32STATE_CONDITIONS_H
