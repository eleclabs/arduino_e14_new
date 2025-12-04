#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4       // ขาเชื่อมต่อ DHT22
#define DHTTYPE DHT22  // ประเภทเซ็นเซอร์

const char* ssid = "A56";
const char* password = "42117622";

// URL API Flask (เปลี่ยนให้ตรงกับ IP ของเครื่องรัน Flask)
String serverName = "http://10.183.22.212:5000/api/data";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  
  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{\"temperature\": " + String(temp) + ", \"humidity\": " + String(hum) + "}";
      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.println("Data sent successfully");
        Serial.println(http.getString());
      } else {
        Serial.print("Error sending data: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // ส่งข้อมูลทุก 5 วินาที
}
