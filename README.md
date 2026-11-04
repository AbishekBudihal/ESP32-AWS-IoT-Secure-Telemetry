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
