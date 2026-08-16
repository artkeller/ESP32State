/**
 * @file ESP32State_Capabilities.h
 * @brief Generated capability guards for ESP32State — replaces the
 *        hand-written ESP32State_HAL.h.
 *
 * GENERATED FILE — do not edit by hand. Regenerate with:
 *   python3 harvester/generate_esp32state_capabilities.py
 *
 * Built from four harvested fact sets, each pulled from the real
 * espressif/esp-idf and espressif/arduino-esp32 repositories rather than
 * hand-typed: esp_reset_reason_t (version axis), esp_sleep_source_t
 * (version axis), soc_caps.h (target axis), idf_component.yml (framework
 * axis). See harvester/README.md for the full method and caveats.
 */

#ifndef ESP32STATE_CAPABILITIES_H
#define ESP32STATE_CAPABILITIES_H

// ============================================================================
// Achse 3: Framework detection (must come first — nothing else works without it)
// ============================================================================
// Framework detection: __has_include, NOT #ifdef ESP32.
// #ifdef ESP32 fails to detect the "Arduino as ESP-IDF component" build
// path (the *only* documented way to use esp32c2/esp32c61 with Arduino) —
// that path does not auto-define ESP32/ARDUINO/ARDUINO_ARCH_ESP32, even
// though <Arduino.h> genuinely exists and is includable. Confirmed via
// espressif/arduino-esp32 issue #3866.
#if __has_include(<Arduino.h>)
    #define ESP32STATE_FRAMEWORK_ARDUINO 1
#else
    #define ESP32STATE_FRAMEWORK_ARDUINO 0
#endif

#if __has_include(<esp_idf_version.h>)
    #include <esp_idf_version.h>
    #define ESP32STATE_HAS_IDF_VERSION 1
#else
    #define ESP32STATE_HAS_IDF_VERSION 0
#endif

// Targets with NO arduino-esp32 support as of the last harvested release
// (master): esp32h4, esp32h21, esp32s31.
// Building ESP32State under Arduino for one of these will fail to find a
// board definition long before this header is even reached — flagged here
// for documentation, not enforced (nothing to #if against on the ESP32State
// side; the failure happens at the board-manager level).

// ============================================================================
// Achse 1a: esp_reset_reason_t — version-gated (ESP_IDF_VERSION_VAL)
// ============================================================================
// Source: https://github.com/espressif/esp-idf/components/esp_system/include/esp_system.h
#define ESP32STATE_HAS_RESET_UNKNOWN 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_POWERON 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_EXT 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_SW 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_PANIC 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_INT_WDT 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_TASK_WDT 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_WDT 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_DEEPSLEEP 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_BROWNOUT 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_RESET_SDIO 1  // present since v5.0 (oldest harvested tag)
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 2, 0)
    #define ESP32STATE_HAS_RESET_USB 1  // first seen in v5.2, commit 11eaf41b37
#else
    #define ESP32STATE_HAS_RESET_USB 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 2, 0)
    #define ESP32STATE_HAS_RESET_JTAG 1  // first seen in v5.2, commit 11eaf41b37
#else
    #define ESP32STATE_HAS_RESET_JTAG 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
    #define ESP32STATE_HAS_RESET_EFUSE 1  // first seen in v5.3, commit e0991facf5
#else
    #define ESP32STATE_HAS_RESET_EFUSE 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
    #define ESP32STATE_HAS_RESET_PWR_GLITCH 1  // first seen in v5.3, commit e0991facf5
#else
    #define ESP32STATE_HAS_RESET_PWR_GLITCH 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
    #define ESP32STATE_HAS_RESET_CPU_LOCKUP 1  // first seen in v5.3, commit e0991facf5
#else
    #define ESP32STATE_HAS_RESET_CPU_LOCKUP 0
#endif

// ============================================================================
// Achse 1b: esp_sleep_source_t — version-gated (ESP_IDF_VERSION_VAL)
// ============================================================================
// Source: https://github.com/espressif/esp-idf/components/esp_hw_support/include/esp_sleep.h
#define ESP32STATE_HAS_WAKEUP_UNDEFINED 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_ALL 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_EXT0 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_EXT1 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_TIMER 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_TOUCHPAD 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_ULP 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_GPIO 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_UART 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_WIFI 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_COCPU 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_COCPU_TRAP_TRIG 1  // present since v5.0 (oldest harvested tag)
#define ESP32STATE_HAS_WAKEUP_BT 1  // present since v5.0 (oldest harvested tag)
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
    #define ESP32STATE_HAS_WAKEUP_VAD 1  // first seen in v5.4, commit 67c1de1eeb
#else
    #define ESP32STATE_HAS_WAKEUP_VAD 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
    #define ESP32STATE_HAS_WAKEUP_VBAT_UNDER_VOLT 1  // first seen in v5.5, commit 8c750b088c
#else
    #define ESP32STATE_HAS_WAKEUP_VBAT_UNDER_VOLT 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
    #define ESP32STATE_HAS_WAKEUP_UART1 1  // first seen in v6.0, commit 662a3be354
#else
    #define ESP32STATE_HAS_WAKEUP_UART1 0
#endif
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
    #define ESP32STATE_HAS_WAKEUP_UART2 1  // first seen in v6.0, commit 662a3be354
#else
    #define ESP32STATE_HAS_WAKEUP_UART2 0
#endif
// --- Not yet guarded: only observed on an unreleased ref. ---
// TODO ESP32STATE_HAS_WAKEUP_UART0  (member: ESP_SLEEP_WAKEUP_UART0, currently only in: master)
// TODO ESP32STATE_HAS_WAKEUP_UART3  (member: ESP_SLEEP_WAKEUP_UART3, currently only in: master)
// TODO ESP32STATE_HAS_WAKEUP_UART4  (member: ESP_SLEEP_WAKEUP_UART4, currently only in: master)
// TODO ESP32STATE_HAS_WAKEUP_USB  (member: ESP_SLEEP_WAKEUP_USB, currently only in: master)

// ============================================================================
// Achse 2: soc_caps.h — target-gated (#if defined(SOC_*), verified names only)
// ============================================================================
// soc_caps.h must be included here — the #if defined(SOC_*) tests below
// only see macros that are already visible at this point.
#if __has_include(<soc/soc_caps.h>)
    #include <soc/soc_caps.h>
#endif
// Source: ESP-IDF master branch, components/soc/<target>/include/soc/soc_caps.h
// CAVEAT: SOC_BROWNOUT_RESET_SUPPORTED on classic esp32 is additionally gated behind '#if SOC_CAPS_ECO_VER >= 100' (a chip silicon revision condition) in the real header — a third axis (chip revision) this harvester does not resolve, only flags.
// SOC_BROWNOUT_DET_SUPPORTED is FICTIONAL — never defined on any harvested target. Not generating a guard for it.
#if defined(SOC_BROWNOUT_RESET_SUPPORTED)
    #define ESP32STATE_HAS_BROWNOUT_RESET 1
#else
    #define ESP32STATE_HAS_BROWNOUT_RESET 0
#endif
#if defined(SOC_BOD_SUPPORTED)
    #define ESP32STATE_HAS_BOD 1
#else
    #define ESP32STATE_HAS_BOD 0
#endif
// SOC_TOUCH_SENSOR_NUM is FICTIONAL — never defined on any harvested target. Not generating a guard for it.
// SOC_TOUCH_SUPPORTED is FICTIONAL — never defined on any harvested target. Not generating a guard for it.
#if defined(SOC_TOUCH_SENSOR_SUPPORTED)
    #define ESP32STATE_HAS_TOUCH_SENSOR 1
#else
    #define ESP32STATE_HAS_TOUCH_SENSOR 0
#endif
#if defined(SOC_PM_SUPPORT_EXT0_WAKEUP)
    #define ESP32STATE_HAS_PM_SUPPORT_EXT0_WAKEUP 1
#else
    #define ESP32STATE_HAS_PM_SUPPORT_EXT0_WAKEUP 0
#endif
#if defined(SOC_PM_SUPPORT_EXT1_WAKEUP)
    #define ESP32STATE_HAS_PM_SUPPORT_EXT1_WAKEUP 1
#else
    #define ESP32STATE_HAS_PM_SUPPORT_EXT1_WAKEUP 0
#endif
#if defined(SOC_RTCIO_HOLD_SUPPORTED)
    #define ESP32STATE_HAS_RTCIO_HOLD 1
#else
    #define ESP32STATE_HAS_RTCIO_HOLD 0
#endif
#if defined(SOC_ULP_SUPPORTED)
    #define ESP32STATE_HAS_ULP 1
#else
    #define ESP32STATE_HAS_ULP 0
#endif
#if defined(SOC_RTC_FAST_MEM_SUPPORTED)
    #define ESP32STATE_HAS_RTC_FAST_MEM 1
#else
    #define ESP32STATE_HAS_RTC_FAST_MEM 0
#endif
#if defined(SOC_RTC_MEM_SUPPORTED)
    #define ESP32STATE_HAS_RTC_MEM 1
#else
    #define ESP32STATE_HAS_RTC_MEM 0
#endif
#if defined(SOC_PM_SUPPORT_CPU_PD)
    #define ESP32STATE_HAS_PM_SUPPORT_CPU_PD 1
#else
    #define ESP32STATE_HAS_PM_SUPPORT_CPU_PD 0
#endif

#endif // ESP32STATE_CAPABILITIES_H