int dir1 = 33;
int dir2 = 32;
int pwm1 = 25;
int dir3 = 26;
int dir4 = 27;
int pwm2 = 14;

int sw = 2;

void setup() {
  Serial.begin(9600);
  pinMode(sw, INPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);  
}

void loop() {
  sw_ok();
  fd(2000);
  stop(1000);
  bk(2000);
  stop(1000);

}

void fd(int dl) {
  Serial.println("Forward");
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4, LOW);
  analogWrite(pwm1, 250);
  analogWrite(pwm2, 250);
  delay(dl);
}

void bk(int dl) {
  Serial.println("Backward");
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, LOW);
  digitalWrite(dir4, HIGH);
  analogWrite(pwm1, 250);
  analogWrite(pwm2, 250);
  
  delay(dl);
}

void stop(int dl){

  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
}

void sw_ok(){
  while(1){
    if(digitalRead(sw) == HIGH){
      break;
    }
  }
}