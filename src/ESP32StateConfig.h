/**
 * @file ESP32StateConfig.h
 * @brief DEPRECATED compatibility shim.
 *
 * Everything this file used to declare (LogLevel, Config, configure(),
 * log(), the global config instance) has moved into ESP32State.h as
 * nested types and static members of `class ESP32State` — see that
 * file's header comment for why (this file's `namespace ESP32State`
 * could not coexist with ESP32State.h's `class ESP32State` in the same
 * translation unit; the original two-file split did not actually
 * compile).
 *
 * This file is kept only so that any code directly doing
 * `#include "ESP32StateConfig.h"` (instead of `#include "ESP32State.h"`)
 * keeps working. New code should include ESP32State.h directly.
 */

#ifndef ESP32STATE_CONFIG_H
#define ESP32STATE_CONFIG_H

#include "ESP32State.h"

#endif // ESP32STATE_CONFIG_H
