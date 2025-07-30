#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// ใช้ขา 10 (RX) และ 11 (TX) สำหรับรับข้อมูลจาก GPS
SoftwareSerial gpsSerial(10, 11);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  Serial.println("Initializing GPS...");
}

void loop() {
  //ดูข้อมูลดิบ NMEA Sentences
  /*
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.print(c);  // แสดงข้อมูล NMEA Sentence
  }*/

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {  // เมื่อได้รับพิกัดใหม่
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Longitude: ");
    Serial.println(gps.location.lng(), 6);
  }

  if (gps.altitude.isUpdated()) {  // เมื่อได้รับค่าความสูงใหม่
    Serial.print("Altitude: ");
    Serial.print(gps.altitude.meters());
  }

  if (gps.speed.isUpdated()) {  // เมื่อได้รับค่าความเร็วใหม่
    Serial.print("Speed: ");
    Serial.print(gps.speed.kmph());
    Serial.println(" km/h");
  }
  
}
