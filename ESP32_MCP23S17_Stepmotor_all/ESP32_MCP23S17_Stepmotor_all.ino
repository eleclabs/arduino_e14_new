#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;
// กำหนดขาที่ใช้
// #define DIR1 27
// #define DIR2 14
// #define DIR3 12
// #define DIR4 13
// #define PWM1 32
// #define PWM2 33
// #define PWM3 25
// #define PWM4 26

#define sw 2  //เอาใว้เทสทั่วไป
//#define sw1 36
//#define sw2 39
//#define sw3 34

//#define sw_j7 16
//#define sw_j6 17
//#define sw_j5 21
//#define sw_j4 30  
//#define sw_j3 30 
//#define sw_j2 30 
//#define sw_j1 30 

#define DIR1 8   // MCP23S17 GPB0
#define STEP1 9  // MCP23S17 GPB1
#define DIR2 10    // MCP23S17 GPB2
#define STEP2 11   // MCP23S17 GPB3
#define DIR3 12    // MCP23S17 GPB4
#define STEP3 13   // MCP23S17 GPB5
#define DIR4 14    // MCP23S17 GPA6
#define STEP4 15   // MCP23S17 GPA7

#define DIR5 7   // MCP23S17 GPB0
#define STEP5 6  // MCP23S17 GPB1
#define DIR6 5   // MCP23S17 GPB2
#define STEP6 4  // MCP23S17 GPB3

#define DIR7 4   // ESP32    4
#define STEP7 15  // ESP32   15

void setup() {
  Serial.begin(115200);

  // เริ่ม MCP23S17 (ใช้ CS = GPIO5)
  if (!mcp.begin_SPI(5)) {
    Serial.println("MCP23S17 ไม่พบ");
    while (1);
  }

  // ตั้งโหมดขาเป็น OUTPUT
  pinMode(sw, INPUT);
  // pinMode(sw1, INPUT);
  // pinMode(sw2, INPUT);
  // pinMode(sw3, INPUT);
  // pinMode(sw_j7, INPUT_PULLUP);
  // pinMode(sw_j6, INPUT_PULLUP);
  // pinMode(sw_j5, INPUT_PULLUP);
  // pinMode(sw_j4, INPUT_PULLUP);
  // pinMode(sw_j3, INPUT_PULLUP);
  // pinMode(sw_j2, INPUT_PULLUP);
  // pinMode(sw_j1, INPUT_PULLUP);

  // pinMode(DIR1, OUTPUT);
  // pinMode(DIR2, OUTPUT);
  // pinMode(DIR3, OUTPUT);
  // pinMode(DIR4, OUTPUT);

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

  pinMode(DIR7, OUTPUT);
  pinMode(STEP7, OUTPUT);
}

void loop() {
  //swGreen();
  //swRed();
  //swOrange();
  sw_ok();

  // step_j1(1000, true);
  // delay(1000);
  // step_j1(1000, false);
  // delay(1000);
  // step_j23(500, false);
  // delay(1000);
  // step_j23(500, true);
  // delay(1000);
  // step_j4(500, true);
  // delay(1000);
  // step_j4(500, false);
  // delay(1000);
  // step_j5(500, true);
  // delay(1000);
  // step_j5(500, false);
  // delay(1000);
  // step_j6(500, true);
  // delay(1000);
  // step_j6(500, false);
  // delay(1000);
  step_j7(1000, true);
  delay(1000);
  step_j7(1000, false);
  delay(1000);
 

}


void step_j1(int steps, bool direction) {
  mcp.digitalWrite(DIR1, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP1, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP1, LOW);
    delayMicroseconds(1000);
  }
}


void step_j23(int steps, bool direction) {
  mcp.digitalWrite(DIR2, direction ? HIGH : LOW);
  mcp.digitalWrite(DIR3, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP2, HIGH);
    mcp.digitalWrite(STEP3, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP2, LOW);
    mcp.digitalWrite(STEP3, LOW);
    delayMicroseconds(1000);
  }
}

void step_j4(int steps, bool direction) {
  mcp.digitalWrite(DIR4, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP4, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP4, LOW);
    delayMicroseconds(1000);
  }
}

void step_j5(int steps, bool direction) {
  mcp.digitalWrite(DIR5, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP5, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP5, LOW);
    delayMicroseconds(1000);
  }
}

void step_j6(int steps, bool direction) {
  mcp.digitalWrite(DIR6, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    mcp.digitalWrite(STEP6, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    mcp.digitalWrite(STEP6, LOW);
    delayMicroseconds(1000);
  }
}

void step_j7(int steps, bool direction) {
  digitalWrite(DIR7, direction ? HIGH : LOW);
  for (int i = 0; i < steps; i++) {
    // if(digitalRead(sw_j7) == LOW){ 
    //   steps = 0;
    //   break; 
    // }
    digitalWrite(STEP7, HIGH);
    delayMicroseconds(1000);  // ความเร็วหมุน (เพิ่มได้ถ้ามอเตอร์สั่น)
    digitalWrite(STEP7, LOW);
    delayMicroseconds(1000);
  }
}

void sw_ok() {
  while (1) {
    Serial.println("OFF");
    if (digitalRead(sw) == HIGH) {
      Serial.println("ON");
      break;
    }
    delay(500);
  }
}

// void swGreen() {
//   while (1) {
//     Serial.println("OFF");
//     if (digitalRead(sw1) == LOW) {
//       Serial.println("ON");
//       break;
//     }
//     delay(500);
//   }
// }

// void swRed() {
//   while (1) {
//     Serial.println("OFF");
//     if (digitalRead(sw2) == LOW) {
//       Serial.println("ON");
//       break;
//     }
//     delay(200);
//   }
// }

// void swOrange() {
//   while (1) {
//     Serial.println("OFF");
//     if (digitalRead(sw3) == LOW) {
//       Serial.println("ON");
//       break;
//     }
//     delay(500);
//   }
// }
