/**
 * @file ESP32State.h
 * @brief Analyzer for ESP32 startup conditions and post-mortem state evaluation.
 */

#ifndef ESP32STATE_H
#define ESP32STATE_H

#ifdef ESP32

#include <functional>
#include <vector>
#include "ESP32StateConfig.h"        // Configurable output stream routing and logging for ESP32State

class ESP32State {
public:
    using Condition = std::function<bool()>;
    using Callback = std::function<void()>;

    struct ConditionPair {
        Condition condition; ///< Condition to evaluate
        Callback callback;   ///< Callback to run on match
    };

    struct AnalysisResult {
        size_t matched;   ///< Number of conditions matched
        size_t unmatched; ///< Number of conditions not matched
    };

    /**
     * @brief Constructor with an optional list of conditions and a default callback.
     */
    ESP32State(const std::vector<ConditionPair>& conditions = {}, Callback defaultCallback = nullptr);

    /**
     * @brief Evaluates all conditions and runs matched callbacks.
     */
    AnalysisResult analyze();

    /**
     * @brief Dynamically adds a condition with its callback.
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
