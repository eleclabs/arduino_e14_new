#define M1_DIR1 3
#define M1_DIR2 2
#define M1_PWM 5

#define M2_DIR1 4
#define M2_DIR2 7
#define M2_PWM 6
int sw = 8;
int led = 13;


void setup() {
  Serial.begin(9600);
  pinMode(M1_DIR1, OUTPUT);
  pinMode(M1_DIR2, OUTPUT);
  pinMode(M2_DIR1, OUTPUT);
  pinMode(M2_DIR2, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  //led_blink();
  
  sw_ok();
  while (1) {
    fd(255, 1000);
    bk(255, 1000);
  }
  
}


void sw_ok() {
  Serial.println("Press OK..");
  while (1) {
    if (digitalRead(sw) == 0) break;
  }
}

void fd(int speed, int dl) {
  Serial.println("Forward");
  digitalWrite(M1_DIR1, HIGH);
  digitalWrite(M1_DIR2, LOW);
  digitalWrite(M2_DIR1, HIGH);
  digitalWrite(M2_DIR2, LOW);
  analogWrite(M1_PWM, speed);
  analogWrite(M2_PWM, speed);
  delay(dl);
}

void bk(int speed, int dl) {
  Serial.println("Backward");
  digitalWrite(M1_DIR1, LOW);
  digitalWrite(M1_DIR2, HIGH);
  digitalWrite(M2_DIR1, LOW);
  digitalWrite(M2_DIR2, HIGH);
  analogWrite(M1_PWM, speed);
  analogWrite(M2_PWM, speed);
  delay(dl);
}

void led_blink(){
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
}
