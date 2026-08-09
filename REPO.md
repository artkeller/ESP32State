````
ESP32State/
├── library.json             # PlatformIO Package Registry Manifest
├── library.properties       # Arduino IDE Package Manifest
├── README.md                # Dokumentation & CRA-Hinweise
├── LICENSE                  # CC0-1.0 / MIT
│
├── src/
│   ├── ESP32State.h         # Core Engine (Interface)
│   ├── ESP32State.cpp       # Core Engine (Implementation & Global Config)
│   ├── ESP32StateConfig.h   # Stream-Routing, CRA Truncation & LogLevel
│   │
│   ├── conditions/          # Vordefinierte Condition-Sets
│   │   ├── Default.h        # Standard Reset- & Wakeup-Checks (SOC_* geguardet)
│   │   └── Power.h          # Spezial-Checks (z. B. VDD-Glitch, Voltage-Drop)
│   │
│   └── variants/            # Chip- / Framework-Spezifika (falls nötig)
│       └── ESP32State_HAL.h # Abstraktion für seltene IDF/Arduino-Abweichungen
│
│
└── examples/
    ├── 01_BasicStartupAnalysis/                
    │   └── 01_BasicStartupAnalysis.ino
    ├── 02_ConditionsInFile/
    │   ├── StartupConditions.h
    │   └── 02_ConditionsInFile.ino
    ├── 03_DynamicConditions/
    │   └── 03_DynamicConditions.ino
    ├── 04_CustomLogger/
    │   └── 04_CustomLogger.ino
    ├── 05_AnalysisResultEvaluation/
    │   └── 05_AnalysisResultEvaluation.ino
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
````
