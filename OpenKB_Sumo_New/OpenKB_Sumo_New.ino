#include <Wire.h>
#include <dummy.h>  // for ESP32
#define SW1 16
#define SW2 14
int BZ = 13;  //ลำโพง Buzzer
int S = 35;   //Sensor พื้น
//int led[4] = { 17, 2, 15, 12 };
#define SDA1 4                  //sda1 in ESP32
#define SCL1 5                  //scl1 in ESP32
#define MOTOR_I2C_ADDRESS 0x50  //จากการตั้งค่า dip_switch
TwoWire I2Ctwo = TwoWire(1);    //ประกาศใช้ I2C ชุดที่ 2 (ESP32 มี I2C 2ชุด)

#define numsensor 3
int Max[numsensor] = { 583, 587, 581 };
int Min[numsensor] = { 0, 0, 0 };
int P[3] = { 32, 33, 34 };
unsigned long s[numsensor];
int error, Last_Error, Integral, leftmotor, rightmotor, PowerMotor, Position;
int last_value = 0;

void setup() {
  Serial.begin(9600);                //Set Serial Monitor
  analogReadResolution(10);          //Set ADC 10 bit 0-1023
  I2Ctwo.begin(SDA1, SCL1, 100000);  //ใช้ I2C ชุดที่ 2 ด้วยความถี่ Clock = 100Khz
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(S, INPUT);    //Sensor พื้น
  pinMode(BZ, OUTPUT);  //Buzzer ลำโพง
                        /*
  for (int i = 0; i <= 3; i++) {    //pinMode LED
    pinMode(led[i], OUTPUT);
  }

  for (int i = 0; i <= 3; i++) {    //LED Blink
    digitalWrite(led[i], LOW);
    delay(100);
    digitalWrite(led[i], HIGH);
    delay(100);
  }

  for (int i = 3; i >= 0; i--) {
    digitalWrite(led[i], LOW);
    delay(100);
    digitalWrite(led[i], HIGH);
    delay(100);
  }
*/

  beep();
  motor1(0);
  motor2(0);
  waitSW1(100);
  beep();
  delay(5000);
  //view();
  //Move(-250,250,4000);
  //Back();
  //END();
}

//white 10+
//black 220+

void loop() {
  // Back();
  //Serial.println( analogRead(S));
  Track3(30, 0.05, 0.04);
  //stdPID(0, 0.05,0.04); //0.05,0.09
}

void beep() {
  analogWrite(BZ, 150);
  delay(90);
  analogWrite(BZ, 0);
}

void motor1(int tSpeed) {
  int tPWM;
  int tDir;
  if (tSpeed > 0) {
    tPWM = tSpeed;
    tDir = 1;
  } else if (tSpeed < 0) {
    tPWM = -tSpeed;
    tDir = 2;
  } else {
    tPWM = 0;
    tDir = 3;
  }
  if (tPWM > 255) tPWM = 255;

  I2Ctwo.beginTransmission(MOTOR_I2C_ADDRESS);
  I2Ctwo.write(0x85);
  I2Ctwo.write(tDir);
  I2Ctwo.write(tPWM);
  I2Ctwo.endTransmission();
}

void motor2(int tSpeed) {
  int tPWM;
  int tDir;
  if (tSpeed > 0) {
    tPWM = tSpeed;
    tDir = 1;
  } else if (tSpeed < 0) {
    tPWM = -tSpeed;
    tDir = 2;
  } else {
    tPWM = 0;
    tDir = 3;
  }
  if (tPWM > 255) tPWM = 255;

  I2Ctwo.beginTransmission(MOTOR_I2C_ADDRESS);
  I2Ctwo.write(0x86);
  I2Ctwo.write(tDir);
  I2Ctwo.write(tPWM);
  I2Ctwo.endTransmission();
}

void Move(int a, int b, int c) {
  motor1(a);
  motor2(b);
  delay(c);
}

void MotorStop(int dl) {
  motor1(0);
  motor2(0);
  delay(dl);
}

void waitSW1(int dl) {
  while (digitalRead(SW1) == HIGH) { delay(dl); }  //รอกดปุ่ม SW1
}

void waitSW2(int dl) {
  while (digitalRead(SW2) == HIGH) { delay(dl); }  //รอกดปุ่ม SW2
}

void END() {
  while (1) { MotorStop(10); }
}

void view() {
  while (1) {
    for (int i = 0; i < numsensor; i++) {
      s[i] = analogRead(P[i]);
      Serial.print(s[i]);
      Serial.print(",\t");
    }
    Serial.println("");
    delay(200);
  }
}

//วนอ่านค่าเซนเซอร์ 10 bit---------------
void Read() {
  for (int i = 0; i < numsensor; i++) {
    int x = map(analogRead(P[i]), Min[i], Max[i], 1000, 0);  // อ่านค่าเซ้นเซอร์ ถ้าไม่เกาะเส้นให้เปลี่ยน  1000, 0
    if (x > 1000) x = 1000;
    if (x < 0) x = 0;
    s[i] = x;
  }
}

// เอาค่าเซนเซอร์มาระบุตำแหน่ง-------------
int ReadLine() {
  int k;
  int on_line = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  on_line = 0;
  Read();
  avg = 0;
  sum = 0;
  for (int i = 0; i < numsensor; i++) {
    if (s[i] > 200) on_line = 1;
    if (s[i] > 50) {
      avg += s[i] * (i * 1000);
      sum += s[i];
    }
  }

  if (on_line == 0) {
    if (last_value < ((numsensor - 1) * 1000) / 2) {
      return 0;
    } else {
      return (numsensor - 1) * 1000;
    }
  }
  last_value = avg / sum;
  return last_value;
}

void stdPID(int BaseSpeed, float Kp, float Kd) {  //  แท็กเส้นดำ
  Position = ReadLine();
  error = Position - 1000;  //1000 เซนเซอร์ 3 ตัว, 3500 เซนเซอร์ 8 ตัว
  PowerMotor = (Kp * error) + (Kd * (error - Last_Error));
  Last_Error = error;
  leftmotor = BaseSpeed + PowerMotor;
  rightmotor = BaseSpeed - PowerMotor;
  if (leftmotor > 250) leftmotor = 250;
  if (leftmotor < -250) leftmotor = -250;
  if (rightmotor > 250) rightmotor = 250;
  if (rightmotor < -250) rightmotor = -250;
  Move(leftmotor, rightmotor, 0);
}

void TrackTime(int BaseSpeed, float Kp, float Kd, int Timer) {
  unsigned long Last_Time = millis();
  while ((millis() - Last_Time) <= Timer) {
    stdPID(BaseSpeed, Kp, Kd);
  }
  MotorStop(10);
}

void Track(int BaseSpeed, float Kp, float Kd) {  // อยู่ในแท็กเค้า  แท็กจนจะเจอเส้นตัด
  ReadLine();
  while (s[1] > 400) {          //ไม่เจอวัตถุ
    if (analogRead(S) > 220) {  //อ่านพื้น เจอขาว?  <
      Back();
    }
    stdPID(BaseSpeed, Kp, Kd);  //วิ่งช้าไม่ให้หลุดเส้นขาว
    //Serial.println("Finding enemy...");
  }
  /*
  while (s[1] == 0) {  //เจอวัตถุ < 400
    if (analogRead(S) > 220){ //<    s<50 = white  s>220 = black
      Back();
    }
    stdPID(200, 0.05, 0.03);  //วิ่งเร็วหาวัตถุ  
    //Serial.println("Enemy sighted! Approaching enemy!!");
  }
*/
}

void Track2(int BaseSpeed, float Kp, float Kd) {  // อยู่ในแท็กเค้า  แท็กจนจะเจอเส้นตัด
  if (analogRead(S) < 50) {                       //เจอเส้นขาว (ดำค่ามาก  ขาวค่าน้อย)
    ReadLine();
    if (s[0] == 0 || s[1] == 0 || s[2] == 0) {  //เจอวัตถุ
      stdPID(0, Kp, Kd);                        //วิ่งช้าไม่ให้หลุดเส้นขาว
    } else {
      stdPID(BaseSpeed, Kp, Kd);  //วิ่งช้าไม่ให้หลุดเส้นขาว
    }
  } else if (analogRead(S) > 200) {  //เจอเส้นดำ (ดำค่ามาก  ขาวค่าน้อย)
    Back();
  }
}

void Track3(int BaseSpeed, float Kp, float Kd) {  // อยู่ในแท็กเค้า  แท็กจนจะเจอเส้นตัด
  if (analogRead(S) < 200) {                       //เจอเส้นขาว (ดำค่ามาก  ขาวค่าน้อย)
    ReadLine();
    while (s[1] == 0 ) {  //เจอวัตถุ
      ReadLine();
      stdPID(200, Kp, Kd);                           //วิ่งช้าไม่ให้หลุดเส้นขาว
      //if (analogRead(S) > 200) {                   //เจอเส้นดำ
      //  Back();
      //}
    }
    stdPID(BaseSpeed, Kp, Kd);  //วิ่งช้าไม่ให้หลุดเส้นขาว
  } else if (analogRead(S) > 200) {  //เจอเส้นดำ (ดำค่ามาก  ขาวค่าน้อย)
    Back();
  }
}


void Back() {
  Move(-150, -150, 500);
  Uturn();
  //Serial.println("Reversing...");
}

void Uturn() {  // เลี้ยวซ้าย จนกว่าจะเจอเส้น
  Move(-100, 100, 200);
}
