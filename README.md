# ESP32 AWS IoT Core Secure Telemetry Engine

An enterprise-ready embedded IoT client built on ESP32 that securely transmits telemetry data to AWS IoT Core over TLS-encrypted MQTT (Port 8883), featuring health diagnostic monitoring and JSON payload versioning.

---

## Architecture Overview

```text
+-------------------+       TLS 1.2 / MQTT (8883)       +---------------------+
|  ESP32 Edge Node  | ================================> |  AWS IoT Core Engine |
+-------------------+                                   +---------------------+
  - Free Heap Mon                                                  ||
  - RSSI / Uptime                                                  \/
  - Auto-reconnect                                      +---------------------+
                                                        | MQTT Test Client /  |
                                                        | DynamoDB / Shadow   |
                                                        +---------------------+
---
## Key Features
### TLS 1.2 Authentication: Mutual X.509 certificate authentication securing hardware-to-cloud communications.
### Non-Blocking Reconnection: Resilient Wi-Fi and MQTT auto-reconnect loops to handle edge network drops.
### System Diagnostics: Streams live heap availability, Wi-Fi RSSI (dBm), and system uptime alongside sensor metrics.
### Payload Versioning: Structured JSON scheme ready for AWS Rules Engine parsing and Timestream/DynamoDB ingestion.
---

## Repository Structure

├── src/
│   ├── main.cpp (or ESP32_AWS_IoT.ino)
│   └── secrets_template.h    # Template for Wi-Fi & AWS X.509 Certificates
├── docs/
│   └── architecture.png
├── .gitignore
└── README.md
---
