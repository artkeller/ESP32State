```text
ESP32State/
├── library.json             # PlatformIO package registry manifest
├── library.properties       # Arduino IDE package manifest
├── README.md                # Documentation & CRA notes
├── CRA-EXEMPTION.md         # CRA applicability statement for downstream users
├── LICENSE                  # MIT
│
├── src/
│   ├── ESP32State.h             # Core engine (interface)
│   ├── ESP32State.cpp           # Core engine (implementation & global config)
│   ├── ESP32StateConfig.h       # Stream routing & LogLevel
│   ├── README.md                # Source-level documentation
│   │
│   ├── conditions/              # Predefined condition sets
│   │   ├── ESP32State_Defaults.h    # Standard reset & wakeup checks (ESP32STATE_HAS_* guarded)
│   │   └── ESP32State_PowerDown.h   # Pre-sleep power-down domain auditor
│   │
│   └── variants/                # Chip-/framework-specific abstraction
│       ├── ESP32State_Capabilities.h  # GENERATED capability guards — see harvester/
│       └── ESP32State_HAL.h           # DEPRECATED shim (HAL is now ESP32State::HAL, a nested struct)
│
├── harvester/                   # Fact-harvesting scripts + generator for
│                                 # ESP32State_Capabilities.h — see harvester/README.md
│
└── examples/
    ├── 01_BasicUsage/
    │   └── 01_BasicUsage.ino
    ├── 02_ModularExample/
    │   ├── RestartConditions.h
    │   └── 02_ModularExample.ino
    ├── 03_AdvancedConditions/
    │   ├── RestartConditions.h
    │   └── 03_AdvancedConditions.ino
    ├── 04_ErrorCounterInNVS/
    │   └── 04_ErrorCounterInNVS.ino
    ├── 05_ErrorCounterWithDefault/
    │   └── 05_ErrorCounterWithDefault.ino
    ├── 06_PersistentErrorHandling/
    │   ├── StartupConditions.h
    │   └── 06_PersistentErrorHandling.ino
    ├── 07_DeepSleepRTCTracking/
    │   ├── RestartConditions.h
    │   └── 07_DeepSleepRTCTracking.ino
    ├── 08_EnvironmentConditions/
    │   ├── EnvironmentConditions.h
    │   └── 08_EnvironmentConditions.ino
    ├── 09_SafetyAndSecurity/
    │   ├── SafetyConditions.h
    │   └── 09_SafetyAndSecurity.ino
    ├── 10_ComprehensiveConditions/
    │   ├── ESP32State_Conditions.h
    │   └── 10_ComprehensiveConditions.ino
    └── README.md
```
