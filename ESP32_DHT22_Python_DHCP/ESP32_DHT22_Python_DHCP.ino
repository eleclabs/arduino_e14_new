#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "A56";
const char* password = "42117622";
const char* serverName = "http://192.168.59.212:5000/api/sensor";

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (isnan(temp) || isnan(humid)) {
    Serial.println("Read failed!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humid) + "}";
    int httpResponseCode = http.POST(payload);

    Serial.println("Sent: " + payload);
    Serial.print("Response: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(10000); // ส่งทุก 10 วินาที
}
