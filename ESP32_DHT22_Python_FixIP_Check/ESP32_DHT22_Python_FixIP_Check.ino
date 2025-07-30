#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "A56";
const char* password = "42117622";

// Fix IP
IPAddress local_IP(192, 168, 59, 200);
IPAddress gateway(192, 168, 59, 159);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// URL API Server (Flask)
const char* serverURL = "http://192.168.59.212:5000/api/sensor";  // เปลี่ยนให้ตรงกับ IP Flask

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println(" Failed to configure static IP");
  }

  WiFi.begin(ssid, password);
  Serial.print(" Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n WiFi connected");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (isnan(temp) || isnan(humid)) {
    Serial.println(" Failed to read from DHT sensor! ");
    //delay(5000);
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Check if Flask Server is reachable
    http.begin(serverURL);
    //int checkResponse = http.GET();

    //if (checkResponse == 200 || checkResponse == 404) {
      // Server is reachable, proceed to POST
      http.addHeader("Content-Type", "application/json");
      //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String payload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humid) + "}";
      int httpCode = http.POST(payload);
      Serial.printf(" POST %s → Code: %d\n", payload.c_str(), httpCode);
    //} else {
    //  Serial.printf(" Server unreachable! GET status: %d\n", checkResponse);
    //}

    http.end();
  } else {
    Serial.println(" WiFi not connected! ");
  }

  delay(10000);
}

