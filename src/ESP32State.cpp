/**
 * @file ESP32State.cpp
 * @brief Implementation of the ESP32State analyzer.
 */

#include "ESP32State.h"

// Definition of the static class member (was: a global inside
// "namespace ESP32State { Config globalConfig; }" — now a static member
// of the class instead, since the class/namespace split is gone).
ESP32State::Config ESP32State::globalConfig;

ESP32State::ESP32State(
    const std::vector<ConditionPair>& conditions,
    Callback defaultCallback)
    : conditions_(conditions), defaultCallback_(defaultCallback) {}

ESP32State::AnalysisResult ESP32State::analyze() {
    AnalysisResult result = {0, 0, 0};

    for (const auto& entry : conditions_) {
        if (entry.condition()) {
            if (entry.callback) {
                entry.callback();
            }
            result.matched++;
        } else {
            result.unmatched++;
        }
    }
    result.total_evaluated = result.matched + result.unmatched;

    if (result.matched == 0 && defaultCallback_) {
        defaultCallback_();
    }

    return result;
}

void ESP32State::addCondition(const Condition& condition, const Callback& callback) {
    conditions_.push_back({condition, callback});
}
