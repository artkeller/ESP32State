#ifndef ESP32STATE_HAL_H
#define ESP32STATE_HAL_H

#include <sdkconfig.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <soc/soc_caps.h> // Target capabilities from Espressif

/**
 * @file ESP32State_HAL.h
 * @brief Zero-Overhead Hardware Abstraction Layer for ESP32State.
 * 
 * Unifies target-specific SOC capabilities and reset/wakeup reasons across
 * all 14 ESP32 SoC variants (ESP32, S2, S3, C2, C3, C5, C6, C61, H2, H4, P4, E22, H21, S31).
 * 
 * @see https://github.com/artkeller/ESP32Features for full feature matrix reference.
 */

// ============================================================================
// 1. HARDWARE CAPABILITY DESCRIPTORS (Compile-Time)
// ============================================================================

// --- Sleep & Wakeup Capabilities ---
#if defined(SOC_PM_SUPPORT_EXT0_WAKEUP) || defined(SOC_RTCIO_HOLD_SUPPORTED)
  #define ESP32STATE_HAS_EXT0_WAKEUP 1
#else
  #define ESP32STATE_HAS_EXT0_WAKEUP 0
#endif

#if defined(SOC_PM_SUPPORT_EXT1_WAKEUP)
  #define ESP32STATE_HAS_EXT1_WAKEUP 1
#else
  #define ESP32STATE_HAS_EXT1_WAKEUP 0
#endif

#if defined(SOC_TOUCH_SENSOR_NUM) && (SOC_TOUCH_SENSOR_NUM > 0)
  #define ESP32STATE_HAS_TOUCH_WAKEUP 1
#else
  #define ESP32STATE_HAS_TOUCH_WAKEUP 0
#endif

#if defined(SOC_ULP_SUPPORTED)
  #define ESP32STATE_HAS_ULP_WAKEUP 1
#else
  #define ESP32STATE_HAS_ULP_WAKEUP 0
#endif

// --- Memory & Power Diagnostics ---
#if defined(SOC_RTC_FAST_MEM_SUPPORTED) || defined(SOC_RTC_MEM_SUPPORTED)
  #define ESP32STATE_HAS_RTC_MEM 1
#else
  #define ESP32STATE_HAS_RTC_MEM 0
#endif

#if defined(SOC_BROWNOUT_DET_SUPPORTED)
  #define ESP32STATE_HAS_BROWNOUT_DET 1
#else
  #define ESP32STATE_HAS_BROWNOUT_DET 0
#endif


// ============================================================================
// 2. TARGET SOC IDENTIFICATION (All 14 Variants)
// ============================================================================

namespace ESP32State {
namespace HAL {

    /**
     * @brief Returns the exact SoC target name at compile-time.
     */
    constexpr const char* getTargetName() {
#if defined(CONFIG_IDF_TARGET_ESP32)
        return "ESP32";
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
        return "ESP32-S2";
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
        return "ESP32-S3";
#elif defined(CONFIG_IDF_TARGET_ESP32S31)
        return "ESP32-S31";
#elif defined(CONFIG_IDF_TARGET_ESP32C2)
        return "ESP32-C2";
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
        return "ESP32-C3";
#elif defined(CONFIG_IDF_TARGET_ESP32C5)
        return "ESP32-C5";
#elif defined(CONFIG_IDF_TARGET_ESP32C6)
        return "ESP32-C6";
#elif defined(CONFIG_IDF_TARGET_ESP32C61)
        return "ESP32-C61";
#elif defined(CONFIG_IDF_TARGET_ESP32H2)
        return "ESP32-H2";
#elif defined(CONFIG_IDF_TARGET_ESP32H4)
        return "ESP32-H4";
#elif defined(CONFIG_IDF_TARGET_ESP32H21)
        return "ESP32-H21";
#elif defined(CONFIG_IDF_TARGET_ESP32P4)
        return "ESP32-P4";
#elif defined(CONFIG_IDF_TARGET_ESP32E22)
        return "ESP32-E22";
#else
        return "ESP32 (Generic/Unknown Target)";
#endif
    }

    /**
     * @brief Agnostic read-only getter for system reset cause.
     */
    inline esp_reset_reason_t getResetReason() {
        return esp_reset_reason();
    }

    /**
     * @brief Agnostic read-only getter for sleep wake-up cause.
     */
    inline esp_sleep_source_t getWakeupCause() {
        return esp_sleep_get_wakeup_cause();
    }

} // namespace HAL
} // namespace ESP32State

#endif // ESP32STATE_HAL_H
