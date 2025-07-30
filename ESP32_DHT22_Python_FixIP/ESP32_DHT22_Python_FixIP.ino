#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// กำหนด WiFi
const char* ssid = "A56";
const char* password = "42117622";

// กำหนด Fixed IP (ต้องอยู่ในวงเดียวกับ WiFi Router)
IPAddress local_IP(192, 168, 59, 200);     // IP ที่จะให้ ESP32 ใช้
IPAddress gateway(192, 168, 59, 159);       // IP ของ Router
IPAddress subnet(255, 255, 255, 0);       // Subnet mask
IPAddress dns(8, 8, 8, 8);                // DNS server

// Server Flask API (เปลี่ยนให้ตรงกับ IP ของเครื่องที่รัน Flask)
const char* serverName = "http://192.168.59.212:5000/api/sensor";

void setup() {
  Serial.begin(115200);
  dht.begin();

  // กำหนด Fixed IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("⚠️ Failed to configure static IP");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected");
  Serial.print("📡 IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (isnan(temp) || isnan(humid)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    delay(5000);
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humid) + "}";
    int httpResponseCode = http.POST(payload);

    Serial.println("📤 Sent: " + payload);
    Serial.print("🧾 Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("❌ WiFi not connected!");
  }

  delay(10000); // ส่งข้อมูลทุก 10 วินาที
}
