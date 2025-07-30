int led = 2;
int sw = 36;
#define PWMA 32
#define DIR1A 33
#define DIR2A 25
#define PWMB 13
#define DIR1B 12
#define DIR2B 14

void setup() {
  pinMode(led, OUTPUT);
  pinMode(sw, INPUT);
  pinMode(DIR1A, OUTPUT);
  pinMode(DIR2A, OUTPUT);
  pinMode(DIR1B, OUTPUT);
  pinMode(DIR2B, OUTPUT);

}

void loop() {
  sw_ok();
  fd(128, 2000);
  bk(128,1000);
  stop(10);
}

void blink() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}

void fd(int speed, int dl) {
  digitalWrite(DIR1A, HIGH);
  digitalWrite(DIR2A, LOW);
  digitalWrite(DIR1B, LOW);
  digitalWrite(DIR2B, HIGH);
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
  delay(dl);
}

void bk(int speed, int dl) {
  digitalWrite(DIR1A, LOW);
  digitalWrite(DIR2A, HIGH);
  digitalWrite(DIR1B, HIGH);
  digitalWrite(DIR2B, LOW);
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
  delay(dl);
}

void stop(int dl){
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void sw_ok(){
  while(1){
    blink();
    if(digitalRead(sw)== LOW){
      break;
    } 
  }
}