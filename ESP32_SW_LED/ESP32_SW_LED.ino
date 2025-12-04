int sw = 36;
int led = 2;
void setup() {
  pinMode(sw, INPUT);
  pinMode(led, OUTPUT);

}

void loop() {
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);

} 
