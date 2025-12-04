// กำหนดขา LED Relay
#define RY1_PIN 5
#define RY2_PIN 17
#define RY3_PIN 16
#define RY4_PIN 4
#define RY5_PIN 15

String cmd = "";

void setup() {
  Serial.begin(115200);

  pinMode(RY1_PIN, OUTPUT);
  pinMode(RY2_PIN, OUTPUT);
  pinMode(RY3_PIN, OUTPUT);
  pinMode(RY4_PIN, OUTPUT);
  pinMode(RY5_PIN, OUTPUT);

  // ปิดไฟทั้งหมดตอนเริ่มต้น
  digitalWrite(RY1_PIN, LOW);
  digitalWrite(RY2_PIN, LOW);
  digitalWrite(RY3_PIN, LOW);
  digitalWrite(RY4_PIN, LOW);
  digitalWrite(RY5_PIN, LOW);

  Serial.println("Ready! พิมพ์คำสั่ง: open, off, ry1, ry2, ry3, ry4, ry5");
}

void loop() {
  // เช็คถ้ามีข้อมูลจาก Serial
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n'); // อ่านจนเจอ Enter
    cmd.trim(); // ตัด space และ \r\n

    if (cmd == "on") {
      // เปิดไฟขาว = R+G+B
      digitalWrite(RY1_PIN, HIGH);
      digitalWrite(RY2_PIN, HIGH);
      digitalWrite(RY3_PIN, HIGH);
      digitalWrite(RY4_PIN, HIGH);
      digitalWrite(RY5_PIN, HIGH);     
      Serial.println("Relay = ON");
    } 
    else if (cmd == "off") {
      // ปิดไฟทั้งหมด
      digitalWrite(RY1_PIN, LOW);
      digitalWrite(RY2_PIN, LOW);
      digitalWrite(RY3_PIN, LOW);
      digitalWrite(RY4_PIN, LOW);
      digitalWrite(RY5_PIN, LOW);
      Serial.println("Relay = OFF");
    } 
    else if (cmd == "ry1") {
      digitalWrite(RY1_PIN, HIGH);
      digitalWrite(RY2_PIN, LOW);
      digitalWrite(RY3_PIN, LOW);
      digitalWrite(RY4_PIN, LOW);
      digitalWrite(RY5_PIN, LOW);
      Serial.println("Relay1 = ON");
    } 
    else if (cmd == "ry2") {
      digitalWrite(RY1_PIN, LOW);
      digitalWrite(RY2_PIN, HIGH);
      digitalWrite(RY3_PIN, LOW);
      digitalWrite(RY4_PIN, LOW);
      digitalWrite(RY5_PIN, LOW);
      Serial.println("Relay2 = ON");
    } 
    else if (cmd == "ry3") {
      digitalWrite(RY1_PIN, LOW);
      digitalWrite(RY2_PIN, LOW);
      digitalWrite(RY3_PIN, HIGH);
      digitalWrite(RY4_PIN, LOW);
      digitalWrite(RY5_PIN, LOW);
      Serial.println("Relay3 = ON");
    } 
    else if (cmd == "ry4") {
      digitalWrite(RY1_PIN, LOW);
      digitalWrite(RY2_PIN, LOW);
      digitalWrite(RY3_PIN, LOW);
      digitalWrite(RY4_PIN, HIGH);
      digitalWrite(RY5_PIN, LOW);
      Serial.println("Relay4 = ON");
    } 
    else if (cmd == "ry5") {
      digitalWrite(RY1_PIN, LOW);
      digitalWrite(RY2_PIN, LOW);
      digitalWrite(RY3_PIN, LOW);
      digitalWrite(RY4_PIN, LOW);
      digitalWrite(RY5_PIN, HIGH);
      Serial.println("Relay5 = ON");
    } 
    else {
      Serial.println("Unknown command: " + cmd);
    }
  }
}
