#include <ESP32Servo.h>
#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;
Servo myservo;  //ประกาศตัวแปรแทน Servo
int pos = 0;
int servoPin = 22;

// กำหนดขาที่ใช้
int dir1 = 27;  //DIR หน้าซ้าย
int dir2 = 14;  //DIR หน้าขวา
int dir3 = 12;  //DIR หลังซ้าย
int dir4 = 13;  //DIR หลังขวา
int pwm1 = 32;  //PWM หน้าซ้าย
int pwm2 = 33;  //PWM หลังขวา
int pwm3 = 25;  //PWM หน้าซ้าย
int pwm4 = 26;  //PWM หลังขวา

int swR = 39;  //Active LOW
int swG = 34;  //Active LOW
int swO = 35;  //Active LOW
int sw = 2;    //Active HIGH

int sw_j2 = 0;   //MCP23S17 GPA0  สวิตช์ล่าง
int sw_j3 = 1;   //MCP23S17 GPA1  สวิตช์บน
int sw_j4 = 3;   //MCP23S17 GPA2  สวิตช์ joy4
int sw_j5 = 0;   //ESP32 0        สวิตช์ joy5
int sw_j6 = 16;  //ESP32 16       สวิตช์ joy6
int sw_j7 = 36;  //ESP32 36       สวิตช์ joy7

#define DIR1 8    // MCP23S17 GPB0
#define STEP1 9   // MCP23S17 GPB1
#define DIR2 10   // MCP23S17 GPB2
#define STEP2 11  // MCP23S17 GPB3
#define DIR3 12   // MCP23S17 GPB4
#define STEP3 13  // MCP23S17 GPB5
#define DIR4 14   // MCP23S17 GPB6
#define STEP4 15  // MCP23S17 GPB7
#define DIR5 7    // MCP23S17 GPA7
#define STEP5 6   // MCP23S17 GPA6
#define DIR6 5    // MCP23S17 GPA5
#define STEP6 4   // MCP23S17 GPA4
#define DIR7 21   // ESP32  21
#define STEP7 17  // ESP32 17

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

  // ตั้งโหมดขาเป็น INPUT
  pinMode(sw, INPUT);
  pinMode(swR, INPUT);
  pinMode(swG, INPUT);
  pinMode(swO, INPUT);

  mcp.pinMode(sw_j2, INPUT);
  mcp.pinMode(sw_j3, INPUT);
  mcp.pinMode(sw_j4, INPUT);
  pinMode(sw_j5, INPUT);  //ESP32 0
  pinMode(sw_j6, INPUT);  //ESP32 16
  pinMode(sw_j7, INPUT);  //ESP32 36

  // ตั้งโหมดขาเป็น OUTPUT
  mcp.pinMode(DIR1, OUTPUT);
  mcp.pinMode(STEP1, OUTPUT);
  mcp.pinMode(DIR2, OUTPUT);
  mcp.pinMode(STEP2, OUTPUT);
  mcp.pinMode(DIR3, OUTPUT);
  mcp.pinMode(STEP3, OUTPUT);
  mcp.pinMode(DIR4, OUTPUT);
  mcp.pinMode(STEP4, OUTPUT);
  mcp.pinMode(DIR5, OUTPUT);
  mcp.pinMode(STEP5, OUTPUT);
  mcp.pinMode(DIR6, OUTPUT);
  mcp.pinMode(STEP6, OUTPUT);
  pinMode(DIR7, OUTPUT);   //ESP32 21
  pinMode(STEP7, OUTPUT);  //ESP32 17

  pinMode(dir1, OUTPUT);  //ESP32 27
  pinMode(dir2, OUTPUT);  //ESP32 14
  pinMode(dir3, OUTPUT);  //ESP32 12
  pinMode(dir4, OUTPUT);  //ESP32 13
}

void loop() {
  //sw_ok();    //อยู่บนบอร์ด ESP32 ใว้ทดสอบทั่วไป
  swGreen();
  //swRed();
  //swOrange();

  //servo_grip();  //คีบ
  //servo_put();   //วาง

  fd(128, 1000);    //fd(ความเร็ว, ดีเลย์);
  stop(1000);
  bk(128,1000);
  stop(1000);
  tl(128,500);
  stop(1000);
  tr(128,500);
  stop(1000);

  stepJ1(1000, true);  //หมุนตามเข็ม ถอยหลัง
  delay(1000);
  stepJ1(1000, false);  //หมุนทวนเข็ม ไปหน้า
  delay(1000);

  stepJ23(5000, true);  //หมุนตามเข็ม ขึ้น
  delay(1000);
  stepJ23X(200, false);  //หมุนทวนเข็ม ลง
  delay(1000);

  stepJ4(1000, true);  //หมุนตามเข็ม ขวา
  delay(1000);
  stepJ4X(200, false); //หมุนทวนเข็ม ซ้าย
  delay(1000);

  stepJ5(1000, true);  //หมุนตามเข็ม ขวา  
  delay(1000);
  stepJ5X(200, false);  //หมุนทวนเข็ม  ซ้าย
  delay(1000);

  stepJ6(1000, true);  //หมุนตามเข็ม ขวา
  delay(1000);
  stepJ6X(200, false); //หมุนทวนเข็ม ซ้าย
  delay(1000);

  stepJ7(5000, true);  //หมุนตามเข็ม ขึ้น
  delay(1000);
  stepJ7X(200, false);  //หมุนทวนเข็ม ลง
  delay(1000);
}

void stepJ1(int steps, bool direction) {
  mcp.digitalWrite(DIR1, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP1, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP1, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ23(int steps, bool direction) {
  //digitalWrite(DIR_PIN, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR2, direction ? LOW : HIGH);
  mcp.digitalWrite(DIR3, direction ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    if (mcp.digitalRead(sw_j2) == LOW) {  //เช็คสวิตช์ด้านล่าง
      steps = 0;
      break;
    }

    if (mcp.digitalRead(sw_j3) == LOW) {  //เช็คสวิตช์ด้านบน
      steps = 0;
      break;
    }
    mcp.digitalWrite(STEP2, HIGH);
    mcp.digitalWrite(STEP3, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP2, LOW);
    mcp.digitalWrite(STEP3, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ4(int steps, bool direction) {
  mcp.digitalWrite(DIR4, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    if (mcp.digitalRead(sw_j4) == LOW) {  //เช็คสวิตช์ joy4
      steps = 0;
      break;
    }
    mcp.digitalWrite(STEP4, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP4, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ5(int steps, bool direction) {
  mcp.digitalWrite(DIR5, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    if (digitalRead(sw_j5) == LOW) {  //เช็คสวิตช์ joy5
      steps = 0;
      break;
    }
    mcp.digitalWrite(STEP5, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP5, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ6(int steps, bool direction) {
  //mcp.digitalWrite(DIR6, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR6, direction ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    if (digitalRead(sw_j6) == LOW) {  //เช็คสวิตช์ joy6
      steps = 0;
      break;
    }
    mcp.digitalWrite(STEP6, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP6, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ7(int steps, bool direction) {
  digitalWrite(DIR7, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    if (digitalRead(sw_j7) == LOW) {  //เช็คสวิตช์ joy7
      steps = 0;
      break;
    }
    digitalWrite(STEP7, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    digitalWrite(STEP7, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ23X(int steps, bool direction) {
  //digitalWrite(DIR_PIN, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR2, direction ? LOW : HIGH);
  mcp.digitalWrite(DIR3, direction ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP2, HIGH);
    mcp.digitalWrite(STEP3, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP2, LOW);
    mcp.digitalWrite(STEP3, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ4X(int steps, bool direction) {
  mcp.digitalWrite(DIR4, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP4, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP4, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ5X(int steps, bool direction) {
  mcp.digitalWrite(DIR5, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP5, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP5, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ6X(int steps, bool direction) {
  //mcp.digitalWrite(DIR6, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR6, direction ? LOW : HIGH);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP6, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP6, LOW);
    delayMicroseconds(1000);
  }
}

void stepJ7X(int steps, bool direction) {
  digitalWrite(DIR7, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP7, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    digitalWrite(STEP7, LOW);
    delayMicroseconds(1000);
  }
}

void sw_ok() {
  while (1) {
    Serial.println("OFF");
    delay(200);
    if (digitalRead(sw) == HIGH) {
      Serial.println("ON");
      break;
    }
  }
}

void swGreen() {
  while (1) {
    Serial.println("OFF");
    delay(200);
    if (digitalRead(swG) == LOW) {
      Serial.println("ON");
      break;
    }
  }
}

void swRed() {
  while (1) {
    Serial.println("OFF");
    delay(200);
    if (digitalRead(swR) == LOW) {
      Serial.println("ON");
      break;
    }
  }
}

void swOrange() {
  while (1) {
    Serial.println("OFF");
    delay(200);
    if (digitalRead(swO) == LOW) {
      Serial.println("ON");
      break;
    }
  }
}

void fd(int speed, int dl) {  //เดินหน้า
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, speed);
  analogWrite(pwm2, speed);
  analogWrite(pwm3, speed);
  analogWrite(pwm4, speed);
  delay(dl);
}

void bk(int speed, int dl) {  //ถอยหลัง
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, speed);
  analogWrite(pwm2, speed);
  analogWrite(pwm3, speed);
  analogWrite(pwm4, speed);
  delay(dl);
}

void tl(int speed, int dl) {  //เลี้ยวซ้าย
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, speed);
  analogWrite(pwm2, speed);
  analogWrite(pwm3, speed);
  analogWrite(pwm4, speed);
  delay(dl);
}

void tr(int speed, int dl) {  //เลี้ยวขวา
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, speed);
  analogWrite(pwm2, speed);
  analogWrite(pwm3, speed);
  analogWrite(pwm4, speed);
  delay(dl);
}

void stop(int dl) {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
  delay(dl);
}

void servo_put() {
  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}

void servo_grip() {
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}