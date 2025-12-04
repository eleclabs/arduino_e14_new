// กำหนดขา LED RGB
#define RED_PIN   23
#define GREEN_PIN 22
#define BLUE_PIN  19

String cmd = "";

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // ปิดไฟทั้งหมดตอนเริ่มต้น
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  Serial.println("Ready! พิมพ์คำสั่ง: open, off, red, green, blue");
}

void loop() {
  // เช็คถ้ามีข้อมูลจาก Serial
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n'); // อ่านจนเจอ Enter
    cmd.trim(); // ตัด space และ \r\n

    if (cmd == "on") {
      // เปิดไฟขาว = R+G+B
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      Serial.println("LED = WHITE");
    } 
    else if (cmd == "off") {
      // ปิดไฟทั้งหมด
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      Serial.println("LED = OFF");
    } 
    else if (cmd == "red") {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      Serial.println("LED = RED");
    } 
    else if (cmd == "green") {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      Serial.println("LED = GREEN");
    } 
    else if (cmd == "blue") {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      Serial.println("LED = BLUE");
    } 
    else {
      Serial.println("Unknown command: " + cmd);
    }
  }
}
