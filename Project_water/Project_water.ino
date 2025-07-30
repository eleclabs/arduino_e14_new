int sw = 2;
int DIR1 = 32;
int PWM1 = 33;
int DIR2 = 25;
int PWM2 = 26;


void setup() {
  pinMode(sw, INPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
}

void loop() {
  while (1) {
    if (digitalRead(sw) == HIGH) {
      break;
    }
  }

  fd(128, 2000);
  stop();
}


void fd(int speed, int dl) {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM1, speed);
  analogWrite(PWM2, speed);
  delay(dl);
}
void bw(int speed, int dl) {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  analogWrite(PWM1, speed);
  analogWrite(PWM2, speed);
  delay(dl);
}
void r(int speed, int dl) {
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, LOW);
  analogWrite(PWM1, speed);
  analogWrite(PWM2, speed);
  delay(dl);
}
void l(int speed, int dl) {
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM1, speed);
  analogWrite(PWM2, speed);
  delay(dl);
}
void stop() {
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
}

void sw_ok() {
  while (1) {
    if (digitalRead(sw) == HIGH) {
      break;
    }
  }
}