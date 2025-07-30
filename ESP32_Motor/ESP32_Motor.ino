int dir1 = 14;
int pwm1 = 12;
int dir2 = 27;
int pwm2 = 12;

void setup() {
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
}

void loop() {
  fd(2000);
}

void fd(int dl) {
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  analogWrite(pwm1, 200);
  analogWrite(pwm2, 200);

  delay(dl);
}