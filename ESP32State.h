/**
 * @file ESP32State.h
 * @brief Analyzer for ESP32 startup conditions and post-mortem state evaluation.
 */

#ifndef ESP32STATE_H
#define ESP32STATE_H

#ifdef ESP32

#include <functional>
#include <vector>
#include "ESP32StateConfig.h"

class ESP32State {
public:
    using Condition = std::function<bool()>;
    using Callback = std::function<void()>;

    struct ConditionPair {
        Condition condition; ///< Evaluierende Bedingung
        Callback callback;   ///< Auszuführender Callback bei Match
    };

    struct AnalysisResult {
        size_t matched;   ///< Anzahl erfüllter Bedingungen
        size_t unmatched; ///< Anzahl nicht erfüllter Bedingungen
    };

    /**
     * @brief Konstruktor mit optionaler Liste von Bedingungen und Default-Callback.
     */
    ESP32State(const std::vector<ConditionPair>& conditions = {}, Callback defaultCallback = nullptr);

    /**
     * @brief Wertet alle Bedingungen aus und führt gematchte Callbacks aus.
     */
    AnalysisResult analyze();

    /**
     * @brief Fügt dynamisch eine Bedingung mit Callback hinzu.
     */
    void addCondition(const Condition& condition, const Callback& callback);

private:
    std::vector<ConditionPair> conditions_;
    Callback defaultCallback_;
};

#else
#error "ESP32State is specifically designed for ESP32 devices relying on ESP-IDF reset APIs."
#endif // ESP32

#endif // ESP32STATE_H
