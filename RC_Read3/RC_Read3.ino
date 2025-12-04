//RC1=L RC2=R
//グローバル変数の宣言
    volatile int RC1_Pluse = 1500;  //TimeB - TimeA = Pluse(1000-2000microsec)
    volatile int RC2_Pluse = 1500;  //TimeB - TimeA = Pluse(1000-2000microsec)
    volatile int RC1_TimeA = 0;  // turn on micros()
    volatile int RC1_TimeB = 0;  // turn off micros()
    volatile int RC2_TimeA = 0;  // turn on micros()
    volatile int RC2_TimeB = 0;  // turn off micros()
    volatile int RC1_Flag = 0;  // 0 = waiting for the pluse to turn on / 1 = waiting for the pluse to turn off 
    volatile int RC2_Flag = 0;  // 0 = waiting for the pluse to turn on / 1 = waiting for the pluse to turn off 
    volatile int RC1_SFlag = 0;  // serial go flag
    volatile int RC2_SFlag = 0;  // serial go flag
    int RC1_STBuffer = 0;  // RC1 Schmitt trigger buffer
    int RC2_STBuffer = 0;  // RC2 Schmitt trigger buffer
    volatile long SW_WDT = 0;  // Software WatchDogTimer
//定数を命名
    #define RC1_IN 2
    #define RC2_IN 3
    #define RC1_PWMH 9
    #define RC1_DIR 10
    #define RC2_PWMH 11
    #define RC2_DIR 12
    #define LED 13

//初期化の処理-------------------------------------------------------------------------------------------------
void setup()
{
    delay(1000); //電源が入ったら1秒待つ
    pinMode(RC1_IN, INPUT_PULLUP); //RC1_IN
    pinMode(RC2_IN, INPUT_PULLUP); //RC2_IN
    pinMode(RC1_PWMH, OUTPUT);  //RC1_PWMH
    pinMode(RC1_DIR, OUTPUT); //RC1_DIR
    pinMode(RC2_PWMH, OUTPUT); //RC2_PWMH
    pinMode(RC2_DIR, OUTPUT); //RC2_DIR
    pinMode(LED, OUTPUT); //ON BORD LED
    digitalWrite(RC1_PWMH, LOW); // RC1 Brake
    digitalWrite(RC1_DIR, LOW); // RC1 Brake
    digitalWrite(RC2_PWMH, LOW); // RC2 Brake
    digitalWrite(RC2_DIR, LOW); // RC2 Brake
    attachInterrupt(1, RC1_interrupt, CHANGE); //割り込み設定
    attachInterrupt(0, RC2_interrupt, CHANGE); //割り込み設定
    //Serial.begin(115200); //デバッグしないときはシリアル出力をコメントアウト
    SW_WDT =  millis();  // Software WatchDogTimer Reset
}

//メイン処理--------------------------------------------------------------------------------------------------
void loop()
{
  //Software WatchDogTimer---------------------------------------------------------------
    #define SW_WDT_Limit 1000 //Software WatchDogTimer limit
  //SW_WDTのLimitを超えたらRC1とRC2をBrakeにして、SW_WDTがクリアされるのを待つ
  //SW_WDTはRC1かRC2の割り込みによってのみクリアされる
    if (millis() - SW_WDT > SW_WDT_Limit) {
      //RC1とRC2をBrakeにする
      digitalWrite(RC1_PWMH, LOW); // RC1 Brake
      digitalWrite(RC1_DIR, LOW); // RC1 Brake
      digitalWrite(RC2_PWMH, LOW); // RC2 Brake
      digitalWrite(RC2_DIR, LOW); // RC2 Brake
      //SW_WDTがクリアされるまでLEDを点滅させる
      while(millis() - SW_WDT > SW_WDT_Limit){  
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
      }
    }

  //シリアル出力---------------------------------------------------------------------------
   /* //デバッグしないときはシリアル出力をコメントアウト ここから
    if (RC1_Flag == 0 && RC2_Flag == 0 && RC1_SFlag == 1 && RC2_SFlag == 1) {
      digitalWrite(LED, HIGH);
      Serial.print(RC2_Pluse);
      Serial.print(":");
      Serial.print(RC1_Pluse);
      Serial.print(":");
      Serial.print(digitalRead(RC1_PWMH));
      Serial.print(digitalRead(RC1_DIR));
      Serial.print(digitalRead(RC2_PWMH));
      Serial.println(digitalRead(RC2_DIR));
      RC1_SFlag = 0;
      RC2_SFlag = 0;
      digitalWrite(LED, LOW);
    } 
   */ //デバッグしないときはシリアル出力をコメントアウト　ここまで

  //RC信号処理---------------------------------------------------------------------
    #define UpperLimit 1680 //RC1&RC2 Forword limit
    #define LowerLimit 1320 //RC1&RC2 Backword limit
    #define STBuffer   10   //RC1&RC2 Schmitt trigger buffer

  //RC1 Forword
    if (RC1_Pluse + RC1_STBuffer > UpperLimit) {
    digitalWrite(RC1_PWMH, HIGH);
    digitalWrite(RC1_DIR, HIGH);
    RC1_STBuffer = STBuffer;
    }
  //RC1 Backword
    if (RC1_Pluse + RC1_STBuffer < LowerLimit) {
    digitalWrite(RC1_PWMH, HIGH);
    digitalWrite(RC1_DIR, LOW);
    RC1_STBuffer = STBuffer * -1;
    }
  //RC1 Brake
    if (RC1_Pluse + RC1_STBuffer >= LowerLimit && RC1_Pluse + RC1_STBuffer <= UpperLimit) {
    digitalWrite(RC1_PWMH, LOW);
    digitalWrite(RC1_DIR, LOW);
    RC1_STBuffer = 0;
    }

  //RC2 Forword
    if (RC2_Pluse + RC2_STBuffer > UpperLimit) {
    digitalWrite(RC2_PWMH, HIGH);
    digitalWrite(RC2_DIR, LOW);
    RC2_STBuffer = STBuffer;
    }
  //RC2 Backword
    if (RC2_Pluse + RC2_STBuffer < LowerLimit) {
    digitalWrite(RC2_PWMH, HIGH);
    digitalWrite(RC2_DIR, HIGH);
    RC2_STBuffer = STBuffer * -1;
    }
  //RC2 Brake
    if (RC2_Pluse + RC2_STBuffer >= LowerLimit && RC2_Pluse + RC2_STBuffer <= UpperLimit) {
    digitalWrite(RC2_PWMH, LOW);
    digitalWrite(RC2_DIR, LOW);
    RC2_STBuffer = 0;
    }
}

//割り込み処理-------------------------------------------------------------------------------------
//RC1信号解析----------------------------------------------------------------------------
void RC1_interrupt()
{
  //RC1_Pluseの開始位置検出
    if (digitalRead(RC1_IN) == LOW && RC1_Flag == 0) {
      RC1_TimeA = micros();
      RC1_Flag = 1;
    }
  //RC1_Pluseの終了位置検出、RC1_Pluseへ値入力
    if (digitalRead(RC1_IN) == HIGH && RC1_Flag == 1) {
      RC1_TimeB = micros();
      RC1_Flag = 0;
      //入力フィルタ801~2199までOK
      if(800 < RC1_TimeB - RC1_TimeA && RC1_TimeB - RC1_TimeA < 2200){
        RC1_Pluse = RC1_TimeB - RC1_TimeA;
        RC1_SFlag = 1;
        SW_WDT =  millis();  // Software WatchDogTimer Reset
      }
    }
}

//RC2信号解析----------------------------------------------------------------------------
void RC2_interrupt()
{
  //RC2_Pluseの開始位置検出
    if (digitalRead(RC2_IN) == LOW && RC2_Flag == 0) {
      RC2_TimeA = micros();
      RC2_Flag = 1;
    }
  //RC2_Pluseの終了位置検出、RC2_Pluseへ値入力
    if (digitalRead(RC2_IN) == HIGH && RC2_Flag == 1) {
      RC2_TimeB = micros();
      RC2_Flag = 0;
      //入力フィルタ801~2199までOK
      if(800 < RC2_TimeB - RC2_TimeA && RC2_TimeB - RC2_TimeA < 2200){
        RC2_Pluse = RC2_TimeB - RC2_TimeA;
        RC2_SFlag = 1;
        SW_WDT =  millis();  // Software WatchDogTimer Reset
      }
    }
}
