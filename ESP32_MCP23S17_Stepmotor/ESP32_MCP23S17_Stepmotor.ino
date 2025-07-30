#include <Adafruit_MCP23X17.h>
#include <ESP32Servo.h>

Adafruit_MCP23X17 mcp;
Servo myservo;  //ประกาศตัวแปรแทน Servo
int pos = 0;    //ค่าเริ่มต้น position
int servoPin = 4; 

// กำหนดขาที่ใช้
#define sw 2
#define DIR_PIN 0
#define STEP_PIN 1

void setup() {
  Serial.begin(115200);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400);  // attaches the servo on pin 22 to the servo object using default min/max of 1000us and 2000us
  
  // เริ่ม MCP23S17 (ใช้ CS = GPIO5)
  if (!mcp.begin_SPI(5)) {
    Serial.println("MCP23S17 ไม่พบ");
    while (1);
  }

  // ตั้งโหมดขาเป็น OUTPUT
  pinMode(sw, INPUT);
  //pinMode(DIR_PIN, OUTPUT);
  //pinMode(STEP_PIN, OUTPUT);
  mcp.pinMode(DIR_PIN, OUTPUT);
  mcp.pinMode(STEP_PIN, OUTPUT);

  myservo.writeMicroseconds(1000); // เริ่มต้นด้วย 0 speed
  delay(3000); // รอ ESC ready
}

void loop() {
  sw_ok();

  myservo.writeMicroseconds(2000) ;
  delay(5000);
  Serial.print("yes ");

  //servo_grip();  //คีบ
  //servo_put();   //วาง

/*
  // หมุนตามเข็ม
  rotateStepper(1000, true);
  delay(1000);
  // หมุนทวนเข็ม
  rotateStepper(1000, false);
  delay(1000);
*/
}

void rotateStepper(int steps, bool direction) {
  //digitalWrite(DIR_PIN, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR_PIN, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    //digitalWrite(STEP_PIN, HIGH);
    mcp.digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    //digitalWrite(STEP_PIN, LOW);
    mcp.digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void sw_ok(){
    while (1) {
    Serial.println("OFF");
    delay(200);
    if (digitalRead(sw) == HIGH) {
      Serial.println("ON");
      break;
    }
  }
}


void servo_put() {
  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(3);                           // waits 15ms for the servo to reach the position
  }
}

void servo_grip() {
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(3);                           // waits 15ms for the servo to reach the position
  }
}
