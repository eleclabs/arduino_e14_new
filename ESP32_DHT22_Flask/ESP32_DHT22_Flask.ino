#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4        // ขาเชื่อมต่อ DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASS";

String serverName = "http://YOUR_PC_IP:5000/api/data"; // Flask API endpoint

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if(WiFi.status()== WL_CONNECTED) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
    }
    http.end();
  }
  delay(10000); // ส่งทุก 10 วินาที
}
