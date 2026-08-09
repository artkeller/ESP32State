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
└── examples/                # Lauffähige Beispiele
    ├── 01_BasicDefault/
    │   └── 01_BasicDefault.ino
    ├── 02_CustomRouting/
    │   └── 02_CustomRouting.ino
    └── 03_ProductionSilent/
        └── 03_ProductionSilent.ino
