#include <ESP32Servo.h>
Servo esc;  //ประกาศตัวแปรแทน Servo
int escPin = 22; 

int sw = 2;

void setup() {
  Serial.begin(9600);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  esc.setPeriodHertz(50);           // standard 50 hz servo
  esc.attach(escPin, 1000, 2000);   // attaches the servo on pin 22 to the servo object using default min/max of 1000us and 2000us
  
  esc.writeMicroseconds(1000);      // เริ่มต้นด้วย 0 speed
  delay(3000); // รอ ESC ready

}

void loop() {
  sw_ok();

  esc.writeMicroseconds(2000) ;     //สั่งให้ ESC ทำงาน
  delay(4000);

  esc.writeMicroseconds(1000) ;     //สั่งให้ ESC หยุดทำงาน
  delay(1000);

}


void sw_ok(){
    while (1) {
    Serial.println("OFF");
    if (digitalRead(sw) == HIGH) {
      Serial.println("ON");
      delay(200);     // ป้องกันการเด้งของปุ่ม
      break;
    }
  }
}

