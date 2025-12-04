#include <POP32.h>
#include <MCP3XXX.h>
MCP3008 adc;
#define NUM_SENSORS 8
uint8_t F_PIN[NUM_SENSORS] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int F[NUM_SENSORS];
int REFmin[NUM_SENSORS], REFmax[NUM_SENSORS];
int  last_value = 0, LastError , SumError;

///////////////////////////////////////////////////////////////////////////
int MinValue[NUM_SENSORS] = {85,198,77,62,75,65,73,53};
int MaxValue[NUM_SENSORS] = {852,894,842,841,869,864,876,774};
int REF[NUM_SENSORS] = {468,546,459,451,472,464,474,413};
///////////////////////////////////////////////////////////////////////////
int MinValue2[NUM_SENSORS] = {548,658,558,496,568,539,556,302};
int MaxValue2[NUM_SENSORS] = {873,914,874,848,876,863,888,800};
int REF2[NUM_SENSORS] = {710,786,716,672,722,701,722,551};
///////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  //AutoRef();
  beep();
  sw_OK_press(); // Wait For Press SW
  beep();
  delay(100);

 //ShowCalibrate(); //Calibrate Light of Sensor
 //SerialF(); //Serial Sensor
 //Program1();
}

void loop() {
//Motor(20,20);delay(1000); 
//stdPID(20, 0.11, 0.03, 100, -10) ; //speed, kp, kd, Max, Min
//FF(20, 0.09, 0.05, 'p', 20, 100, -20);
FFtimer2(20, 0.09, 0.05, 1000, 100, 0); //เริ่มต้นออกจากเส้นเขียว
m1();

while(1){MotorStop();}  //จบการทำงาน

}

//------ SENSOR -----
void ReadSensor() {  //ค่าดิบ
  for (int _sensor = 0; _sensor < NUM_SENSORS; _sensor++) {
    //adc.begin(cs, mosi, miso, sck);
    adc.begin(PA0, PA1, PA2, PA3);
    //F[_sensor] = analog10(F_PIN[_sensor]);
    F[_sensor] = adc.analogRead(F_PIN[_sensor]);
  }
}

void readCalibrate() { //ค่าที่ Calibrate แล้ว
  ReadSensor();
  for (int i = 0; i < NUM_SENSORS; i++) {
    unsigned int calmin, calmax;
    long x = 0;
    calmin = MinValue[i];
    calmax = MaxValue[i];
    x = map(F[i], calmin, calmax, 1000, 0);   //ถ้าไม่เกาะเส้นให้แก้ 0, 1000 หรือ 1000, 0
    if (x < 0) x = 0;
    if (x > 1000) x = 1000;
    F[i] = x;
  }
}

void CalibrateSensor(int pauseTime, int samples) {
  for (int i = 0; i < NUM_SENSORS; i++) {
    MinValue[i] = 1023;
    MaxValue[i] = 0;
  }
  for (int startSamp = 0; startSamp <= samples; startSamp++) {
    ReadSensor();
    for (int i = 0; i < NUM_SENSORS; i++) {
      MinValue[i] = (F[i] <= MinValue[i] ? F[i] : MinValue[i]);
      MaxValue[i] = (F[i] >= MaxValue[i] ? F[i] : MaxValue[i]);
    }
    delay(pauseTime);
  }
}

////////////////////////////////////////////////////////////////
void readCalibrate2() { //ค่าที่ Calibrate แล้ว
  ReadSensor();
  for (int i = 0; i < NUM_SENSORS; i++) {
    unsigned int calmin, calmax;
    long x = 0;
    calmin = MinValue2[i];
    calmax = MaxValue2[i];
    x = map(F[i], calmin, calmax, 1000, 0);   //ถ้าไม่เกาะเส้นให้แก้ 0, 1000 หรือ 1000, 0
    if (x < 0) x = 0;
    if (x > 1000) x = 1000;
    F[i] = x;
  }
}

void CalibrateSensor2(int pauseTime, int samples) {
  for (int i = 0; i < NUM_SENSORS; i++) {
    MinValue2[i] = 1023;
    MaxValue2[i] = 0;
  }
  for (int startSamp = 0; startSamp <= samples; startSamp++) {
    ReadSensor();
    for (int i = 0; i < NUM_SENSORS; i++) {
      MinValue2[i] = (F[i] <= MinValue2[i] ? F[i] : MinValue2[i]);
      MaxValue2[i] = (F[i] >= MaxValue2[i] ? F[i] : MaxValue2[i]);
    }
    delay(pauseTime);
  }
}
///////////////////////////////////////////////////////////////////


void ShowCalibrate() { // Calibrate ค่าแสง
  Serial.println("Press OK to Start Calibrate Sensor");
  sw_OK_press(); // Wait For Press SW
  Serial.println("Calibrating...");
  beep();
  CalibrateSensor(20, 250);
  beep();
  Serial.println("Finish");
  Serial.println("  ");

  Serial.print("int MinValue[NUM_SENSORS] = {");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(MinValue[i]);
    if (i < NUM_SENSORS - 1) Serial.print(",");
    else Serial.println("};");
    REFmin[i] = MinValue[i];
  }

  Serial.print("int MaxValue[NUM_SENSORS] = {");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(MaxValue[i]);
    if (i < NUM_SENSORS - 1) Serial.print(",");
    else Serial.println("};");
    REFmax[i] = MaxValue[i];
  }
  Serial.print("int REF[NUM_SENSORS] = {");
  for (int i = 0; i < NUM_SENSORS; i++) {
    REF[i] = (REFmin[i] + REFmax[i]) / 2;
    Serial.print(REF[i]);
    if (i < NUM_SENSORS - 1) Serial.print(",");
    else Serial.println("};");

  }
  while (1);
}

void AutoRef() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    REF[i] = (MinValue[i] + MaxValue[i]) / 2;
  }
}

void SerialCalibrate() {
  while (1) {
    readCalibrate();
    for (int _serialF = 0; _serialF < NUM_SENSORS; _serialF++) {
      Serial.print(F[_serialF]);
      Serial.print("\t");
    }
    Serial.println("");
    delay(50);
  }
}

void SerialF() {
  while (1) {
    ReadSensor();
    for (int _serialF = 0; _serialF < NUM_SENSORS; _serialF++) {
      Serial.print(F[_serialF]);
      Serial.print("\t");
    }
    Serial.println("");
    delay(50);
  }
}

void SerialPosition() {
  while (1) {
    int Pos = readPosition(250, 50);
    int Error_F = Pos - 3500;
    Serial.print("Pos = ");
    Serial.println(Pos);
    Serial.print("                  Error = ");
    Serial.println(Error_F);
  }
}

void Motor(int leftsp, int rightsp) {
  motor(1, leftsp);
  motor(2, rightsp);
}

void MotorStop() {
  Motor(0, 0); delay(10);
  //ao(); delay(10);
}


//----- PID ------
int readPosition(int keepTrack, int noise) {
  unsigned char i, online = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  static int  last_value = 0;
  readCalibrate();
  for (i = 0 ; i < NUM_SENSORS; i++) {
    int values = F[i];
    if (values > keepTrack) {
      online = 1;
    }
    if (values > noise) {
      avg += (long)(values) * (i * 1000);
      sum += values;
    }
  }
  if (!online) {
    if (last_value < (NUM_SENSORS - 1) * 1000 / 2) {
      return 0;
    } else {
      return (NUM_SENSORS - 1) * 1000;
    }
  }
  last_value = avg / sum ;
  return last_value;
}

void stdPID(int baseSpeed, float  Kp, float Kd, int maxsp, int minsp) {
  int Pos = readPosition(250, 50);
  int Error = Pos - 3500;  //3500
  int PID_Value = (Kp * Error) + (Kd * (Error - LastError));
  LastError = Error;
  int LeftPower = baseSpeed - PID_Value;
  int RightPower = baseSpeed + PID_Value;
  if (LeftPower > 100) LeftPower = maxsp;
  if (LeftPower < 0) LeftPower =  minsp;
  if (RightPower > 100) RightPower = maxsp;
  if (RightPower < 0) RightPower =  minsp;
  Motor(LeftPower, RightPower);
}

/////////////////////////////////////////////////////////////////////
int readPosition2(int keepTrack, int noise) {
  unsigned char i, online = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  static int  last_value = 0;
  readCalibrate2();
  for (i = 0 ; i < NUM_SENSORS; i++) {
    int values = F[i];
    if (values > keepTrack) {
      online = 1;
    }
    if (values > noise) {
      avg += (long)(values) * (i * 1000);
      sum += values;
    }
  }
  if (!online) {
    if (last_value < (NUM_SENSORS - 1) * 1000 / 2) {
      return 0;
    } else {
      return (NUM_SENSORS - 1) * 1000;
    }
  }
  last_value = avg / sum ;
  return last_value;
}

void stdPID2(int baseSpeed, float  Kp, float Kd, int maxsp, int minsp) {
  int Pos = readPosition2(250, 50);
  int Error = Pos - 3500;  //3500
  int PID_Value = (Kp * Error) + (Kd * (Error - LastError));
  LastError = Error;
  int LeftPower = baseSpeed - PID_Value;
  int RightPower = baseSpeed + PID_Value;
  if (LeftPower > 100) LeftPower = maxsp;
  if (LeftPower < 0) LeftPower =  minsp;
  if (RightPower > 100) RightPower = maxsp;
  if (RightPower < 0) RightPower =  minsp;
  Motor(LeftPower, RightPower);
}


void FFtimer(int baseSpeed, float Kp, float Kd, int totalTime, int maxsp, int minsp) {
  unsigned long startTime  = millis();
  unsigned long endTime = startTime + totalTime;
  while (millis() <= endTime) {
    stdPID(baseSpeed, Kp , Kd , maxsp, minsp);
  }
}


void FFtimer2(int baseSpeed, float Kp, float Kd, int totalTime, int maxsp, int minsp) {
  unsigned long startTime  = millis();
  unsigned long endTime = startTime + totalTime;
  while (millis() <= endTime) {
    stdPID2(baseSpeed, Kp , Kd , maxsp, minsp);
  }
}


void FF(int spd, float kp , float kd, char select, int dl, int maxsp, int minsp) {
  while (1) {
    readCalibrate();
    if ((F[0] <= REF[0] && F[1] <= REF[1] && F[2] <= REF[2] && F[3] <= REF[3] && F[4] <= REF[4] && F[5] <= REF[5] && F[6] <= REF[6] &&  F[7] <= REF[7]))
    {
      break;
    }
    stdPID(spd, kp, kd , maxsp, minsp);
  }
  MotorStop();
  if (select == 's') {
    Motor(-spd, -spd); delay(dl);
    while (1)
    {
      readCalibrate();
      if (F[0] <= REF[0] && F[7] <= REF[7]) break;
      Motor(-20, -20);
    }
    MotorStop();
  } else if (select == 'p') {
    Motor(spd, spd); delay(dl);
    while (1)
    {
      readCalibrate();
      if (F[0] <= REF[0] && F[7] <= REF[7]) break;
      Motor(20, 20);
    }
    MotorStop();

  } else if (select == 'l') {
    Motor(spd, spd); delay(dl);
    SpinL();
  } else if (select == 'r') {
    Motor(spd, spd); delay(dl);
    SpinR();
  }
}


/////////////////////////////////////////////////////////////////
void FF2(int spd, float kp , float kd, char select, int dl, int maxsp, int minsp) {
  while (1) {
    readCalibrate2();
   //if ((F[0] <= REF2[0] && F[1] <= REF2[1] && F[2] <= REF2[2] && F[3] <= REF2[3] && F[4] <= REF2[4] && F[5] <= REF2[5] && F[6] <= REF2[6] &&  F[7] <= REF2[7]))
   if ((F[0] <= REF2[0] && F[1] <= REF2[1]  && F[6] <= REF2[6] &&  F[7] <= REF2[7]))
    {
      break;
    }
    stdPID2(spd, kp, kd , maxsp, minsp);
  }

  MotorStop();
  if (select == 's') {
    Motor(-spd, -spd); delay(dl);
    while (1)
    {
      readCalibrate2();
      if (F[0] <= REF2[0] && F[7] <= REF2[7]) break;
      Motor(-20, -20);
    }
    MotorStop();
  } else if (select == 'p') {
    Motor(spd, spd); delay(dl);
    while (1)
    {
      readCalibrate2();
      if (F[0] <= REF2[0] && F[7] <= REF2[7]) break;
      Motor(20, 20);
    }
    MotorStop();

  } else if (select == 'l') {
    Motor(spd, spd); delay(dl);
    SpinL();
  } else if (select == 'r') {
    Motor(spd, spd); delay(dl);
    SpinR();
  }
}
///////////////////////////////////////////////////////////////////


void FFL(int spd, float kp , float kd, char select, int dl, int maxsp, int minsp) {
  while (1) {
    readCalibrate();
    if (((F[0] <= REF[0]  && F[4] <= REF[4]) && (F[6] >= REF[6] &&   F[7] >= REF[7]))) 
    {
      break;
    }
    stdPID(spd, kp, kd , maxsp, minsp);
  }
  MotorStop();
}

void FFR(int spd, float kp , float kd, char select, int dl, int maxsp, int minsp) {
  while (1) {
    readCalibrate();
    if (((F[7] <= REF[7] && F[3] <= REF[3]) &&(F[0] >= REF[0] && F[4] >= REF[4] )))
    {
      break;
    }
    stdPID(spd, kp, kd , maxsp, minsp);
  }
  MotorStop();
}

void FFC(int spd, float kp , float kd) {
  while (1) {
    readCalibrate();
    if ((F[3] <= REF[3] || F[4] <= REF[4] || F[5] <= REF[5]))
    {
      break;
    }
    Motor(spd, spd); delay(50);
  }
  MotorStop();
}

void SpinL() {
  //Motor(100, 100); delay(100); MotorStop();
  Motor(-40, 40); delay(300);
  while (1) {
    readCalibrate();
    if (F[1] <= REF[1]) {
      MotorStop();
      Motor(-20, 20); delay(10);
      break;
    }
    Motor(20, -20);
  }
  MotorStop();
}

void SpinR() {
  //Motor(100, 100); delay(100); MotorStop();
  Motor(40, -40); delay(300); 
  while (1) {
    readCalibrate();
    if (F[6] <= REF[6]) {
      MotorStop();
      Motor(20, -20); delay(10);
      break;
    }
    Motor(-20, 20);
  }
  MotorStop();
}


void SerialPos() {
  while (1) {
    int Pos = readPosition(250, 50);
    int Error_F = Pos - 3500;
    Serial.print("Pos = ");
    Serial.print(Pos);
    Serial.print("                  Error = ");
    Serial.println(Error_F);
  }
}

