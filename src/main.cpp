#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

// Topics for AWS IoT Core
#define AWS_IOT_TOPIC "esp32/telemetry"
#define AWS_SHADOW_UPDATE "$aws/things/ESP32_Test_Device/shadow/update"

WiFiClientSecure net;
PubSubClient client(net);

unsigned long lastMillis = 0;
const int PUBLISH_INTERVAL_MS = 5000;
int reconnectionAttempts = 0;

void setupWifi() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[Init] Connecting to Enterprise Edge Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[Init] Wi-Fi Connected Successfully.");
  Serial.print("[Network] IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("[Network] RSSI (Signal Strength): ");
  Serial.println(WiFi.RSSI());
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT Inbound] Message arrived on topic: ");
  Serial.println(topic);
  
  // Parse incoming payload safely
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.println("[Error] Failed to parse incoming JSON payload.");
    return;
  }
  
  // Handle cloud directives or shadow deltas here
  Serial.println("[MQTT Inbound] Processed command packet successfully.");
}

void connectAWS() {
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

  Serial.println("[Cloud] Attempting secure connection to AWS IoT Core (mTLS)...");

  while (!client.connected()) {
    String clientId = "ESP32_Edge_Node_" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("[Cloud] Connected to AWS IoT Core securely over Port 8883.");
      // Resubscribe to shadow updates or control channels
      client.subscribe("$aws/things/ESP32_Test_Device/shadow/update/accepted");
      reconnectionAttempts = 0;
    } else {
      reconnectionAttempts++;
      Serial.print("[Error] Failed state rc=");
      Serial.print(client.state());
      Serial.print(". Retrying in 5 seconds... (Attempt ");
      Serial.print(reconnectionAttempts);
      Serial.println(")");
      delay(5000);
    }
  }
}

void publishTelemetry() {
  JsonDocument doc;
  
  // Production Telemetry Schema v1.0
  doc["version"] = "1.0.0";
  doc["device_id"] = "ESP32_Test_Device";
  doc["uptime_sec"] = millis() / 1000;
  
  // System Health Metrics (Crucial for Production Diagnostics)
  JsonObject diagnostics = doc.createNestedObject("diagnostics");
  diagnostics["free_heap_bytes"] = ESP.getFreeHeap();
  diagnostics["wifi_rssi_dbm"] = WiFi.RSSI();
  diagnostics["reconnect_count"] = reconnectionAttempts;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  Serial.print("[Telemetry] Publishing payload to ");
  Serial.print(AWS_IOT_TOPIC);
  Serial.println("...");
  
  if (client.publish(AWS_IOT_TOPIC, jsonBuffer)) {
    Serial.println("[Telemetry] Transmission Successful.");
  } else {
    Serial.println("[Error] Transmission Failed. Buffer overflow or socket drop.");
  }
}

void setup() {
  setupWifi();
  connectAWS();
}

void loop() {
  // Maintain connection state
  if (!client.connected()) {
    connectAWS();
  }
  client.loop();

  // Non-blocking telemetry loop interval
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= PUBLISH_INTERVAL_MS) {
    lastMillis = currentMillis;
    publishTelemetry();
  }
}
