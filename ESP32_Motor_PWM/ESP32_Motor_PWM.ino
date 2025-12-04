int dir1 = 33;
int dir2 = 25;
int pwm1 = 32;
int dir3 = 12;
int dir4 = 14;
int pwm2 = 13;

int sw = 36;
int led = 2;
int vr = 39;

void setup() {
  Serial.begin(9600);
  pinMode(sw, INPUT_PULLUP);
  pinMode(vr,INPUT);
  pinMode(led, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);  

}

void loop() {
  sw_ok();
  fd(1000);
  stop(100);
  bk(1000);
  stop(100);

}

void fd(int dl) {
  Serial.println("Forward");
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, 200);
  analogWrite(pwm2, 200);
  delay(dl);
}

void bk(int dl) {
  Serial.println("Backward");
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, 200);
  analogWrite(pwm2, 200);
  delay(dl);
}

void stop(int dl){
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  delay(10);
}

void sw_ok(){
  while(1){
    if(digitalRead(sw) == LOW){
      break;
    }
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
  }
}