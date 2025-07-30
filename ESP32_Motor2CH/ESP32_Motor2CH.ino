int dir1 = 32;
int dir2 = 33;
int dir3 = 25;
int dir4 = 26;

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
  
  analogWrite(dir1, 128);
  analogWrite(dir3, 128);   
  analogWrite(dir2, 0);  
  analogWrite(dir4, 0);
  delay(dl);
}

void bk(int dl) {
  Serial.println("Backward");
  analogWrite(dir2, 128);
  analogWrite(dir4, 128);        
  analogWrite(dir1, 0);  
  analogWrite(dir3, 0); 
  delay(dl);
}

void stop(int dl) {
  analogWrite(dir1, 0);
  analogWrite(dir2, 0);
  analogWrite(dir3, 0);
  analogWrite(dir4, 0);
}

void sw_ok() {
  while (1) {
    if (digitalRead(sw) == HIGH) {
      break;
    }
  }
}