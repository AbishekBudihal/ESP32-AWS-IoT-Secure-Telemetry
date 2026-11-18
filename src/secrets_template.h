#ifndef SECRETS_H
#define SECRETS_H

#include <pgmspace.h>

// Wi-Fi Credentials
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";

// AWS IoT Core Endpoint
const char AWS_IOT_ENDPOINT[] = "your-endpoint-ats.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
YOUR_AMAZON_ROOT_CA_1_HERE
-----END CERTIFICATE-----
)EOF";

// Device Certificate (.cert.pem)
static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
YOUR_DEVICE_CERTIFICATE_HERE
-----END CERTIFICATE-----
)EOF";

// Device Private Key (.private.key)
static const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
YOUR_PRIVATE_KEY_HERE
-----END RSA PRIVATE KEY-----
)EOF";

#endif
