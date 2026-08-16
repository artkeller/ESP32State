/**
 * @file ESP32State_HAL.h
 * @brief DEPRECATED compatibility shim.
 *
 * This file used to declare `namespace ESP32State { namespace HAL {...} }`
 * — the same namespace/class collision problem as ESP32StateConfig.h
 * (a namespace cannot coexist with `class ESP32State` from ESP32State.h,
 * and C++ does not allow a namespace nested inside a class at all).
 *
 * `HAL` is now `ESP32State::HAL`, a nested struct with static methods,
 * declared directly in ESP32State.h. The `ESP32STATE_HAS_*` capability
 * macros this file used to hand-define (including three that turned out
 * to reference non-existent SOC_* names — see project history) now come
 * from the generated ESP32State_Capabilities.h instead.
 *
 * This file is kept only for code doing
 * `#include <variants/ESP32State_HAL.h>` directly. New code should get
 * both from `#include <ESP32State.h>` (pulls in Capabilities.h
 * automatically; HAL is a member of the ESP32State class).
 */

#ifndef ESP32STATE_HAL_H
#define ESP32STATE_HAL_H

#include "ESP32State_Capabilities.h"
#include "../ESP32State.h"

#endif // ESP32STATE_HAL_H
