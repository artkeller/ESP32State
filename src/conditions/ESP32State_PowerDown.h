/**
 * @file ESP32State_PowerDown.h
 * @brief Pre-Sleep Power-Down Domain Configuration Auditor for ESP32State.
 *
 * Known limitation, unchanged from the original file (documented, not
 * fixed here — out of scope for this pass): the condition lambdas below
 * call esp_sleep_pd_config(), a real hardware side effect, from inside
 * what is elsewhere described as a "side-effect-free check matrix."
 * Flagged in project history; still true.
 */

#ifndef ESP32STATE_POWERDOWN_H
#define ESP32STATE_POWERDOWN_H

#include "../ESP32State.h"

namespace ESP32StateDefaults {

/**
 * @brief Helper function to check a single power domain for errors.
 */
inline bool checkPowerDomainError(esp_err_t err, const char* domainName) {
    if (err != ESP_OK) {
        ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "PD Config failed for %s (0x%X)", domainName, err);
        return true; // Return true = an error occurred
    }
    return false;
}

/**
 * @brief Returns all power-down domain conditions for deep sleep.
 */
inline std::vector<ESP32State::ConditionPair> getPowerDownDomainConditions() {
    std::vector<ESP32State::ConditionPair> conditions;

#if defined(ESP_PD_DOMAIN_RTC8M)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_OFF), "RTC8M"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'RTC8M' powerdown domain."); }
    });
#endif

#if defined(ESP_PD_DOMAIN_RTC_FAST_MEM)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON), "RTC_FAST_MEM"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'RTC_FAST_MEM' powerdown domain."); }
    });
#endif

#if defined(ESP_PD_DOMAIN_RTC_SLOW_MEM)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON), "RTC_SLOW_MEM"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'RTC_SLOW_MEM' powerdown domain."); }
    });
#endif

#if defined(ESP_PD_DOMAIN_RTC_PERIPH)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF), "RTC_PERIPH"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'RTC_PERIPH' powerdown domain."); }
    });
#endif

#if defined(ESP_PD_DOMAIN_XTAL)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF), "XTAL"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'XTAL' powerdown domain."); }
    });
#endif

    // Was: "#if SOC_PM_SUPPORT_CPU_PD && defined(ESP_PD_DOMAIN_CPU)" —
    // now the generated, verified ESP32STATE_HAS_PM_SUPPORT_CPU_PD.
#if ESP32STATE_HAS_PM_SUPPORT_CPU_PD && defined(ESP_PD_DOMAIN_CPU)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_CPU, ESP_PD_OPTION_OFF), "CPU"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'CPU' domain."); }
    });
#endif

#if defined(ESP_PD_DOMAIN_VDDSDIO)
    conditions.push_back({
        []() { return checkPowerDomainError(esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF), "VDDSDIO"); },
        []() { ESP32STATE_LOG(ESP32State::LogLevel::ERROR, "Failed to configure 'VDDSDIO' domain."); }
    });
#endif

    return conditions;
}

/**
 * @brief Runs the power-down domain check directly via the ESP32State engine.
 */
inline ESP32State::AnalysisResult auditPowerDownDomains() {
    ESP32STATE_LOG(ESP32State::LogLevel::INFO, "Starting PowerDownDomains Audit...");

    auto pdConditions = getPowerDownDomainConditions();
    ESP32State auditor(pdConditions, []() {
        ESP32STATE_LOG(ESP32State::LogLevel::INFO, "All PowerDown domain conditions passed successfully.");
    });

    return auditor.analyze();
}

} // namespace ESP32StateDefaults

#endif // ESP32STATE_POWERDOWN_H
